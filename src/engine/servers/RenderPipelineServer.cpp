#include "RenderPipelineServer.h"

#include <algorithm>

#include "RenderServer.h"
#include "SceneServer.h"
#include "TimeServer.h"

using namespace std;

NSE::RenderPipelineServer::RenderPipelineServer()
{

}

NSE::RenderPipelineServer::~RenderPipelineServer()
{

}

void NSE::RenderPipelineServer::RenderFrame()
{
    auto render = RenderServer::Get();
    auto time = TimeServer::Get();
    auto scene = SceneServer::Get();

    // Fill global properties
    const auto globalProperties = render->GetGlobalProperties();

    globalProperties->Time = time->Time();
    globalProperties->DeltaTime = time->Delta();

    // Get all visual entities
    vector<Scene*> scenes;
    vector<NSE_Camera> cameras;

    static unordered_map<size_t, vector<NSE_VisualEntity>> entitiesPerScene = {};
    entitiesPerScene.clear();

    scene->GetAllScenes(scenes);
    for (auto s : scenes)
    {
        vector<NSE_VisualEntity> sceneEntities{};
        s->FindAllEntitiesFromBaseType(sceneEntities);
        s->FindAllEntitiesFromBaseType(cameras);
        entitiesPerScene.emplace(s->GetUID(), sceneEntities);
    }

    // Sort entities
    for (auto& sceneEntitiesPair : entitiesPerScene)
    {
        std::sort(sceneEntitiesPair.second.begin(), sceneEntitiesPair.second.end(), VisualEntity::PriorityCompRef);

        // Sort by render queue
        // Sort by shader
        // Sort by inputs hash
    }

    // Sort cameras
    std::sort(cameras.begin(), cameras.end(), Camera::PriorityCompRef);

    for (const auto& camera : cameras)
    {
        // Clear render target
        switch (camera->clearMode)
        {
            case CAMERA_CLEAR_MODE_COLOR:
                if (camera->colorTarget)
                    render->ClearRenderTextureColor(camera->colorTarget, camera->clearColor);
                else
                    render->PipelineClearRenderTarget(true, true, camera->clearColor, 1.0f);
            break;
            case CAMERA_CLEAR_MODE_DEPTH:
                if (camera->depthTarget)
                    render->ClearRenderTextureDepth(camera->depthTarget, 1.0f);
                else
                    render->PipelineClearRenderTarget(false, true, {}, 1.0f);
            break;
            case CAMERA_CLEAR_MODE_NOTHING:

                break;
        }

        if (!camera->targetScene)
            continue;

        auto& sceneEntities = entitiesPerScene.at(camera->targetScene->GetUID());

        // Render entities
        for (const auto& entity : sceneEntities)
        {
            size_t sceneUid;
            if (!entity->GetSceneUID(sceneUid))
                continue;

            if (camera->targetScene && sceneUid != camera->targetScene->GetUID())
                continue;

            entity->RenderEntity(camera);
        }
    }

    render->Present();
}
