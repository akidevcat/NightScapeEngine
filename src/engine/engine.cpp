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
    _objectServer->DestroyAll();
    delete _inputServer;
    delete _renderPipelineServer;
    delete _renderServer;
    delete _sceneServer;
    delete _timeServer;
    delete _applicationServer;
    delete _assetsServer;
    delete _objectServer;
}

bool NSE::Engine::Initialize(IGame* game, HINSTANCE histance, int screenWidth, int screenHeight, HWND hwnd)
{
    bool result = true;

    _gameInstance = game;
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    _objectServer = new ObjectServer{};
    _assetsServer = new AssetsServer{};
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

    _renderPipelineServer = new RenderPipelineServer{};

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

    _objectServer->Update();

    if (!_applicationServer->Update())
    {
        return false;
    }

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
    _renderPipelineServer->RenderFrame();
}

void NSE::Engine::OnFrameCleanup()
{

}

void NSE::Engine::Shutdown()
{

}
