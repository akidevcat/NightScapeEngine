#include "engine.h"

#include <algorithm>
#include <chrono>

#include "entity/Camera.h"
#include "entity/SceneEntity.h"
#include "entity/VisualEntity.h"
#include "scene/Scene.h"

using namespace std;

Engine::Engine()
{

}

Engine::~Engine()
{
    delete _gameInstance;
    delete _inputServer;
    delete _renderServer;
    delete _sceneServer;
    delete _timeServer;
}

bool Engine::Initialize(IGame* game, HINSTANCE histance, int screenWidth, int screenHeight, HWND hwnd)
{
    bool result = true;

    _gameInstance = game;
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    _timeServer = new TimeServer{};
    if (!_timeServer->Initialize())
    {
        return false;
    }

    _inputServer = new InputServer{};
    if (!_inputServer->Initialize(histance, hwnd, screenWidth, screenHeight))
    {
        return false;
    }

    _renderServer = new RenderServer{};
    if (!_renderServer->Initialize(screenWidth, screenHeight, hwnd))
    {
        return false;
    }

    _sceneServer = new SceneServer{};
    if (!_sceneServer->Initialize())
    {
        return false;
    }

    return true;
}

void Engine::Start()
{
    _gameInstance->Start();
}

bool Engine::UpdateFrame()
{
    _timeServer->BeginFrame();

    OnFrameInput();
    _gameInstance->OnFrameInput();

    OnFrameUpdate();
    _gameInstance->OnFrameUpdate();

    OnFrameRender();
    _gameInstance->OnFrameRender();

    OnFrameCleanup();
    _gameInstance->OnFrameCleanup();

    _timeServer->EndFrame();

    return true;
}

void Engine::OnFrameInput()
{
    _inputServer->Update();

    if (_inputServer->GetLMB())
    {
        _inputServer->SetMouseLocked(true);
    }
    if (_inputServer->GetKey(DIK_ESCAPE))
    {
        _inputServer->SetMouseLocked(false);
    }
}

void Engine::OnFrameUpdate()
{
    vector<Scene*> scenes;
    vector<SceneEntity*> entities;

    _sceneServer->GetAllScenes(scenes);
    for (auto scene : scenes)
    {
        scene->GetAllEntities(entities);
    }

    for (auto entity : entities)
    {
        entity->OnUpdate();
    }
}

void Engine::OnFrameRender()
{
    // Fill global properties
    _renderServer->GetGlobalProperties()->Time = _timeServer->Time();
    _renderServer->GetGlobalProperties()->DeltaTime = _timeServer->Delta();
    _renderServer->PipelineMarkGlobalPropertiesDirty();

    _renderServer->BeginScene(DirectX::XMFLOAT4(0, 0, 0, 1));

    // Get all visual entities
    // Camera* _camera = _sceneServer->GetMainCamera();
    vector<Scene*> scenes;
    vector<VisualEntity*> entities;
    vector<Camera*> cameras;

    _sceneServer->GetAllScenes(scenes);
    for (auto scene : scenes)
    {
        scene->FindAllEntitiesFromBaseType(entities);
        scene->FindAllEntitiesFromBaseType(cameras);
    }

    std::sort(cameras.begin(), cameras.end(), Camera::PriorityComp);

    for (auto camera : cameras)
    {
        if (camera->targetRT)
            _renderServer->ClearRenderTarget(camera->targetRT, {0, 0, 0, 1});

        for (auto entity : entities)
        {
            size_t sceneUid;
            if (!entity->GetSceneUID(sceneUid))
                continue;

            if (camera->targetSene && sceneUid != camera->targetSene->GetUID())
                continue;

            entity->RenderEntity(_renderServer, _timeServer, camera);
        }
    }

    // if (_camera)
    // {
    //     for (auto entity : entities)
    //     {
    //         entity->RenderEntity(_renderServer, _timeServer, _camera);
    //     }
    // }

    _renderServer->EndScene();
}

void Engine::OnFrameCleanup()
{

}

void Engine::Shutdown()
{

}
