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
    // Camera* _camera = _sceneServer->GetMainCamera();
    vector<Scene*> scenes;
    vector<NSE_VisualEntity> entities;
    vector<NSE_Camera> cameras;

    scene->GetAllScenes(scenes);
    for (auto s : scenes)
    {
        s->FindAllEntitiesFromBaseType(entities);
        s->FindAllEntitiesFromBaseType(cameras);
    }

    std::sort(cameras.begin(), cameras.end(), Camera::PriorityCompRef);

    for (const auto& camera : cameras)
    {
        switch (camera->clearMode)
        {
            case CAMERA_CLEAR_MODE_COLOR:
                if (camera->targetRT)
                    render->PipelineClearRenderTexture(camera->targetRT, true, true, camera->clearColor, 1.0f);
                else
                    render->PipelineClearRenderTarget(true, true, camera->clearColor, 1.0f);
            break;
            case CAMERA_CLEAR_MODE_DEPTH:
                if (camera->targetRT)
                    render->PipelineClearRenderTexture(camera->targetRT, false, true, {}, 1.0f);
                else
                    render->PipelineClearRenderTarget(false, true, {}, 1.0f);
            break;
            case CAMERA_CLEAR_MODE_NOTHING:

                break;
        }

        for (const auto& entity : entities)
        {
            size_t sceneUid;
            if (!entity->GetSceneUID(sceneUid))
                continue;

            if (camera->targetSene && sceneUid != camera->targetSene->GetUID())
                continue;

            entity->RenderEntity(camera);
        }
    }

    render->Present();
}
