#include "game.h"

#include <iostream>

#include "../engine/entity/Camera.h"
#include "../engine/entity/FreeCamera.h"
#include "../engine/entity/TriangleVisual.h"

Game::Game()
{

}

Game::~Game()
{
    delete _testShader;
    delete _testMaterial;
    // delete _triangle;
    delete _scene;
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
    static size_t tintId = ShaderUtils::PropertyToID("Tint");

    _engine->GetSceneServer()->CreateScene(_scene);

    auto camera = new FreeCamera{_engine->GetInputServer(), _engine->GetTimeServer()};
    _scene->RegisterEntity(camera);
    _engine->GetSceneServer()->SetMainCamera(camera);

    camera->SetParams(_engine->GetScreenAspect(), 90.0f, 0.1f, 1000.0f, false, 0.0f);
    camera->position = {0, 0, -1};

    _testShader = new Shader{L"Assets/Shaders/Triangle.hlsl"};
    _testShader->Compile(_engine->GetRenderServer()->GetDevice());

    float value = 516.3125612f;

    _testMaterial = new Material{_engine->GetRenderServer()->GetDevice(), _testShader};
    _testMaterial->SetPixelVar(tintId, &value, sizeof(float));

    // _triangle = new TriangleVisual{_engine->GetRenderServer()->GetDevice()};
    // _triangle->renderingMaterial = _testMaterial;

    _triangle = new TriangleVisual{_engine->GetRenderServer()->GetDevice()};
    _scene->RegisterEntity(_triangle);

    _triangle->renderingMaterial = _testMaterial;

    _triangle->position = {0, 0, 1};
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
    _triangle->rotation = DirectX::XMQuaternionRotationAxis({0, 0, 1}, _engine->GetTimeServer()->Time() * 2.0f);
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
