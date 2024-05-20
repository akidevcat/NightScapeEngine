#include "game.h"

#include "../engine/entity/Camera.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Initialize(Engine* engine)
{
    _engine = engine;

    return true;
}

void Game::Shutdown()
{

}

void Game::Start()
{
    Scene* scene;
    _engine->GetSceneServer()->CreateScene(scene);

    // SceneEntity* entA = new SceneEntity{};
    // SceneEntity* entB = new SceneEntity{};
    SceneEntity* entC = new Camera{};

    // scene->RegisterEntity(entA);
    // scene->RegisterEntity(entB);
    scene->RegisterEntity(entC);

    vector<Camera*> test{};

    scene->FindAllEntitiesFromBaseType<Camera>(test);
}

bool Game::UpdateFrame()
{
    return true;
}

void Game::OnFrameInput()
{

}

void Game::OnFrameUpdate()
{

}

void Game::OnFrameRender()
{

}

void Game::OnFrameCleanup()
{

}

bool Game::Render()
{
    return true;
}
