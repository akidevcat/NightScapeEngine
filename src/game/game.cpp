#include "game.h"

#include <iostream>

#include "planetary/Planet.h"
#include "ShipController.h"
#include "StarsCamera.h"
#include "../engine/entity/Camera.h"
#include "../engine/entity/FreeCamera.h"
#include "../engine/entity/SpriteVisual.h"
#include "../engine/entity/TextVisual.h"
#include "../engine/entity/TriangleVisual.h"
#include "../engine/servers/AssetServer.h"

using namespace NSE;

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

    // _cameraRT = CreateObject<RenderTexture>(120, 90, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);
    _renderColorRT = CreateObject<RenderTexture>(120, 90, DXGI_FORMAT_R8G8B8A8_UNORM, false);
    _renderDepthRT = CreateObject<RenderTexture>(120, 90, DXGI_FORMAT_D24_UNORM_S8_UINT, true);

    _presentMaterial->GetPSInputs()->SetResource(mainTexID, _renderColorRT->ResourceView());

    // auto starsCamera = CreateObject<StarsCamera>(camera, _cameraRT);

    auto controller = _scene->Create<ShipController>(_scene, _engine->GetScreenAspect());

    controller->GetCamera()->colorTarget = _renderColorRT;
    controller->GetCamera()->depthTarget = _renderDepthRT;

    controller->GetRadar()->AddTarget(_triangle, {1,1,1.0,1});

    auto starsCamera = _scene->Create<StarsCamera>(controller->GetCamera(), controller);

    // auto test = _scene->Create<VisualMeshEntity>();
    // auto baseShader = CreateObject<Shader>(L"Assets/Shaders/Base.hlsl");
    // baseShader->Compile();
    // auto baseMaterial = CreateObject<Material>(baseShader);
    // test->mesh = NSE::AssetsServer::Get()->LoadMeshAsset("Assets/Models/destructor.obj");
    // test->scale = {0.2, 0.2, 0.2};
    // test->renderingMaterial = _testMaterial;
    // test->position = {0, 0, 100};
    // test->rotation = DirectX::XMQuaternionRotationAxis({0, 1, 0}, 180);

    auto marker = _scene->Create<SpriteVisual>();
    marker->sprite.atlasTexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Atlas.dds");
    marker->sprite.SetRectRectPixel(0, 0, 8, 8);
    marker->isPixelPerfect = true;
    marker->isScreenSpace = false;
    marker->color = {1, 0.4, 0, 1};
    marker->position = _triangle->position;

    // auto text = _scene->Create<TextVisual>();
    // text->SetText("Fuel: 37u");
    // text->SetText("12345");

    Planet::Create(_scene, starsCamera->GetStarsScene(), controller);
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
