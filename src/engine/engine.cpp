#include "engine.h"

#include "entity/Camera.h"
#include "entity/SceneEntity.h"
#include "scene/Scene.h"

Engine::Engine()
{

}

Engine::~Engine()
{
    if (_gameInstance)
    {
        delete _gameInstance;
    }

    if (_inputServer)
    {
        delete _inputServer;
    }

    if (_renderServer)
    {
        delete _renderServer;
    }

    if (_sceneServer)
    {
        delete _sceneServer;
    }
}

bool Engine::Initialize(IGame* game, HINSTANCE histance, int screenWidth, int screenHeight, HWND hwnd)
{
    bool result = true;

    _gameInstance = game;

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
    OnFrameInput();
    _gameInstance->OnFrameInput();

    OnFrameUpdate();
    _gameInstance->OnFrameUpdate();

    OnFrameRender();
    _gameInstance->OnFrameRender();

    OnFrameCleanup();
    _gameInstance->OnFrameCleanup();

    return true;
}

void Engine::OnFrameInput()
{
    _inputServer->Update();
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
    _renderServer->BeginScene(XMFLOAT4(0, 1, 0, 1));



    _renderServer->EndScene();
}

void Engine::OnFrameCleanup()
{

}

void Engine::Shutdown()
{

}
