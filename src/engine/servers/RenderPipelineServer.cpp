#include "RenderPipelineServer.h"

#include <algorithm>

#include "RenderServer.h"
#include "SceneServer.h"
#include "TimeServer.h"
#include "../data/Light.h"
#include "../data/LightsProperties.h"

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
    static unordered_map<size_t, vector<NSE_Light>> lightsPerScene = {};
    entitiesPerScene.clear();
    lightsPerScene.clear();

    scene->GetAllScenes(scenes);
    for (auto s : scenes)
    {
        vector<NSE_VisualEntity> sceneEntities{};
        vector<NSE_Light> sceneLights{};
        s->FindAllEntitiesFromBaseType(sceneEntities);
        s->FindAllEntitiesFromBaseType(sceneLights);
        s->FindAllEntitiesFromBaseType(cameras);
        entitiesPerScene.emplace(s->GetUID(), sceneEntities);
        lightsPerScene.emplace(s->GetUID(), sceneLights);
    }

    // Sort entities
    for (auto& sceneEntitiesPair : entitiesPerScene)
    {
        std::sort(sceneEntitiesPair.second.begin(), sceneEntitiesPair.second.end(), VisualEntity::PriorityCompRef);
    }

    // ToDo Sort lights

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

        // Skip if there's no scene target
        if (!camera->targetScene)
            continue;

        // Get scene lights
        static LightData cameraLights[8];
        int cameraLightCount = 0;

        for (const auto& light : lightsPerScene.at(camera->targetScene->GetUID()))
        {
            // Check frustum intersection
            // ToDo ...

            // Set data
            cameraLights[cameraLightCount].Color = light->lightColor;
            cameraLights[cameraLightCount].Intensity = light->lightIntensity;
            cameraLights[cameraLightCount].PositionCS = (DirectX::XMFLOAT3)(light->position - camera->position);

            if (cameraLightCount++ >= 8)
            {
                break;
            }
        }

        // Copy lights
        memcpy(render->GetLightsProperties()->Lights, cameraLights, sizeof(LightData) * cameraLightCount);
        render->GetLightsProperties()->LightCount = cameraLightCount;

        // Render entities
        for (const auto& entity : entitiesPerScene.at(camera->targetScene->GetUID()))
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
