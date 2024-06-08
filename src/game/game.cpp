#include "game.h"

#include <iostream>

#include "ShipController.h"
#include "StarsCamera.h"
#include "../engine/entity/Camera.h"
#include "../engine/entity/FreeCamera.h"
#include "../engine/entity/TriangleVisual.h"
#include "../engine/servers/AssetServer.h"

using namespace NSE;

Game::Game()
{

}

Game::~Game()
{
    // delete _testShader;
    // delete _testMaterial;
    // delete _triangle;
    // delete _scene;
    // delete _cameraRT;
    // delete _presentCamera;
    // delete _presentShader;
    // delete _presentMaterial;
}

bool Game::Initialize(Engine* engine)
{
    _engine = engine;

    return true;
}

void Game::Shutdown()
{

}

using namespace NSE;

void Game::Start()
{
    static size_t mainTexID = ShaderUtils::PropertyToID("_MainTex");

    _engine->GetSceneServer()->CreateScene(_scene);
    _engine->GetSceneServer()->CreateScene(_presentScene);

    _presentShader = CreateObject<Shader>(L"Assets/Shaders/Display.hlsl");
    _presentShader->Compile();

    _presentMaterial = CreateObject<Material>(_presentShader);

    _presentCamera = _presentScene->Create<Camera>();
    _presentCamera->priority = 100;
    _presentCamera->targetScene = _presentScene;
    // _presentCamera->clearMode = CAMERA_CLEAR_MODE_NOTHING;

    _presentPlane = _presentScene->Create<QuadVisual>();
    _presentPlane->renderingMaterial = _presentMaterial;

    // auto camera = _scene->Create<FreeCamera>();
    // camera->targetScene = _scene;
    // camera->SetParams(_engine->GetScreenAspect(), 60.0f, 0.1f, 1000.0f, false, 0.0f);
    // camera->position = {0, 0, -1};
    // camera->clearMode = CAMERA_CLEAR_MODE_DEPTH;

    _testShader = CreateObject<Shader>(L"Assets/Shaders/Triangle.hlsl");
    _testShader->Compile();

    _testMaterial = CreateObject<Material>(_testShader);

    _triangle = _scene->Create<TriangleVisual>();
    _triangle->renderingMaterial = _testMaterial;
    _triangle->position = {0, 0, 6};

    _cameraRT = CreateObject<RenderTexture>(80, 60, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);



    _presentMaterial->GetPSInputs()->SetResource(mainTexID, _cameraRT->GetSRV());

    // auto starsCamera = CreateObject<StarsCamera>(camera, _cameraRT);

    auto controller = _scene->Create<ShipController>(_scene, _engine->GetScreenAspect());
    controller->GetCamera()->targetRT = _cameraRT;

    auto starsCamera = _scene->Create<StarsCamera>(controller->GetCamera(), _cameraRT);


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
