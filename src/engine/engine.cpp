#include "engine.h"

#include <chrono>

#include "entity/Camera.h"
#include "entity/SceneEntity.h"
#include "entity/VisualEntity.h"
#include "scene/Scene.h"

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
    _renderServer->BeginScene(XMFLOAT4(0, 0, 0, 1));

    // Get all visual entities
    vector<Scene*> scenes;
    vector<VisualEntity*> entities;

    _sceneServer->GetAllScenes(scenes);
    for (auto scene : scenes)
    {
        scene->FindAllEntitiesFromBaseType(entities);
    }

    for (auto entity : entities)
    {
        entity->RenderEntity(_renderServer, _timeServer);
    }

    _renderServer->EndScene();
}

void Engine::OnFrameCleanup()
{

}

void Engine::Shutdown()
{

}
