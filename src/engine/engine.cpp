#include "engine.h"

#include <algorithm>
#include <chrono>

#include "entity/Camera.h"
#include "entity/SceneEntity.h"
#include "entity/VisualEntity.h"
#include "scene/Scene.h"
#include "servers/ApplicationServer.h"

using namespace std;

NSE::Engine::Engine()
{

}

NSE::Engine::~Engine()
{
    delete _inputServer;
    delete _renderServer;
    delete _sceneServer;
    delete _timeServer;
    delete _applicationServer;
    delete _objectServer;
}

bool NSE::Engine::Initialize(IGame* game, HINSTANCE histance, int screenWidth, int screenHeight, HWND hwnd)
{
    bool result = true;

    _gameInstance = game;
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    _objectServer = new ObjectServer{};
    _applicationServer = new ApplicationServer{};

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

void NSE::Engine::Start()
{
    _gameInstance->Start();
}

bool NSE::Engine::UpdateFrame()
{
    if (!_applicationServer->Update())
    {
        return false;
    }

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

void NSE::Engine::OnFrameInput()
{
    _inputServer->Update();

    if (_inputServer->GetLMBDown())
    {
        _inputServer->SetMouseLocked(true);
    }
    if (_inputServer->GetKeyDown(DIK_ESCAPE))
    {
        if (_inputServer->GetMouseLocked())
        {
            _inputServer->SetMouseLocked(false);
        }
        else
        {
            ApplicationServer::Get()->Exit();
        }

    }
}

void NSE::Engine::OnFrameUpdate()
{
    vector<Scene*> scenes;
    vector<obj_ptr<SceneEntity>> entities;

    _sceneServer->GetAllScenes(scenes);
    for (auto scene : scenes)
    {
        scene->GetAllEntities(entities);
    }

    for (const auto& entity : entities)
    {
        entity->OnUpdate();
    }
}

void NSE::Engine::OnFrameRender()
{
    // Fill global properties
    _renderServer->GetGlobalProperties()->Time = _timeServer->Time();
    _renderServer->GetGlobalProperties()->DeltaTime = _timeServer->Delta();
    _renderServer->PipelineMarkGlobalPropertiesDirty();

    _renderServer->BeginScene(DirectX::XMFLOAT4(0, 0, 0, 1));

    // Get all visual entities
    // Camera* _camera = _sceneServer->GetMainCamera();
    vector<Scene*> scenes;
    vector<NSE_VisualEntity> entities;
    vector<NSE_Camera> cameras;

    _sceneServer->GetAllScenes(scenes);
    for (auto scene : scenes)
    {
        scene->FindAllEntitiesFromBaseType(entities);
        scene->FindAllEntitiesFromBaseType(cameras);
    }

    std::sort(cameras.begin(), cameras.end(), Camera::PriorityCompRef);

    for (const auto& camera : cameras)
    {
        if (camera->targetRT)
            _renderServer->ClearRenderTarget(camera->targetRT, {0, 0, 0, 1});

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

    // if (_camera)
    // {
    //     for (auto entity : entities)
    //     {
    //         entity->RenderEntity(_renderServer, _timeServer, _camera);
    //     }
    // }

    _renderServer->EndScene();
}

void NSE::Engine::OnFrameCleanup()
{

}

void NSE::Engine::Shutdown()
{

}
