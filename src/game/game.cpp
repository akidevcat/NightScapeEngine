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
    delete _cameraRT;
    delete _presentCamera;
    delete _presentShader;
    delete _presentMaterial;
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
    // static size_t tintId = ShaderUtils::PropertyToID("Tint");

    _engine->GetSceneServer()->CreateScene(_scene);
    _engine->GetSceneServer()->CreateScene(_presentScene);

    _presentShader = new Shader{L"Assets/Shaders/Display.hlsl"};
    _presentShader->Compile(_engine->GetRenderServer()->GetDevice());

    _presentMaterial = new Material(_engine->GetRenderServer()->GetDevice(), _presentShader);

    _presentCamera = new Camera{};
    _presentScene->RegisterEntity(_presentCamera);
    _presentCamera->priority = 100;
    _presentCamera->targetSene = _presentScene;

    _presentPlane = new FullscreenQuad{_engine->GetRenderServer()->GetDevice()};
    _presentScene->RegisterEntity(_presentPlane);
    _presentPlane->renderingMaterial = _presentMaterial;

    auto camera = new FreeCamera{_engine->GetInputServer(), _engine->GetTimeServer()};
    _scene->RegisterEntity(camera);
    _engine->GetSceneServer()->SetMainCamera(camera);
    camera->targetSene = _scene;

    camera->SetParams(_engine->GetScreenAspect(), 60.0f, 0.1f, 1000.0f, false, 0.0f);
    camera->position = {0, 0, -1};

    _testShader = new Shader{L"Assets/Shaders/Triangle.hlsl"};
    _testShader->Compile(_engine->GetRenderServer()->GetDevice());

    _testMaterial = new Material{_engine->GetRenderServer()->GetDevice(), _testShader};

    _triangle = new TriangleVisual{_engine->GetRenderServer()->GetDevice()};
    _scene->RegisterEntity(_triangle);

    _triangle->renderingMaterial = _testMaterial;

    _triangle->position = {0, 0, 1};

    _cameraRT = new RenderTexture{_engine->GetRenderServer()->GetDevice(), 90, 90, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT};

    camera->targetRT = _cameraRT;

    _presentMaterial->SetPSResource(ShaderUtils::PropertyToID("_MainTex"), _cameraRT->GetSRV());

    auto samplers = _engine->GetRenderServer()->GetDefaultPointSampler();

    // ToDo remove
    _engine->GetRenderServer()->GetDeviceContext()->PSSetSamplers(0, 1, &samplers);
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
