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
    free(_shipSystem);
    free(_inventorySystem);
    free(_navigationSystem);
    free(_levelSystem);
    free(_galaxySystem);
    free(_generationSystem);
    free(_mainSystem);
}

bool Game::Initialize(Engine* engine)
{
    _engine = engine;

    _mainSystem = new MainSystem{engine};
    _generationSystem = new GenerationSystem{};
    _galaxySystem = new GalaxySystem{};
    _levelSystem = new LevelSystem{};
    _navigationSystem = new NavigationSystem{};
    _inventorySystem = new InventorySystem{};
    _shipSystem = new ShipSystem{};

    return true;
}

void Game::Shutdown()
{

}

void Game::Start()
{
    _mainSystem->OnStart();
    _generationSystem->OnStart();
    // _galaxySystem->OnStart();
    _levelSystem->OnStart();
    // _navigationSystem->OnStart();
    // _inventorySystem->OnStart();
    _shipSystem->OnStart();

    // static size_t mainTexID = ShaderUtils::PropertyToID("_MainTex");

    // _engine->GetSceneServer()->CreateScene(_scene);
    // _engine->GetSceneServer()->CreateScene(_presentScene);

    // _presentShader = CreateObject<Shader>(L"Assets/Shaders/Display.hlsl");
    // _presentShader->Compile();
    //
    // _presentMaterial = CreateObject<Material>(_presentShader);

    // _presentCamera = _presentScene->Create<Camera>();
    // _presentCamera->priority = 100;
    // _presentCamera->targetScene = _presentScene;

    // _presentPlane = _presentScene->Create<QuadVisual>();
    // _presentPlane->renderingMaterial = _presentMaterial;

    // _testShader = CreateObject<Shader>(L"Assets/Shaders/Triangle.hlsl");
    // _testShader->Compile();
    //
    // _testMaterial = CreateObject<Material>(_testShader);
    //
    // _triangle = _scene->Create<TriangleVisual>();
    // _triangle->renderingMaterial = _testMaterial;
    // _triangle->position = {0, 0, 6};

    // _renderColorRT = CreateObject<RenderTexture>(120, 90, DXGI_FORMAT_R8G8B8A8_UNORM, false);
    // _renderDepthRT = CreateObject<RenderTexture>(120, 90, DXGI_FORMAT_D24_UNORM_S8_UINT, true);

    // _presentMaterial->GetPSInputs()->SetResource(mainTexID, _renderColorRT->ResourceView());

    // auto controller = _scene->Create<ShipController>(_scene, _engine->GetScreenAspect());
    //
    // controller->GetCamera()->colorTarget = _renderColorRT;
    // controller->GetCamera()->depthTarget = _renderDepthRT;
    //
    // controller->GetRadar()->AddTarget(_triangle, {1,1,1.0,1});

    // auto starsCamera = _scene->Create<StarsCamera>(controller->GetCamera(), controller);

    // auto marker = _scene->Create<SpriteVisual>();
    // marker->sprite.atlasTexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Atlas.dds");
    // marker->sprite.SetRectRectPixel(0, 0, 8, 8);
    // marker->isPixelPerfect = true;
    // marker->isScreenSpace = false;
    // marker->color = {1, 0.4, 0, 1};

    // Planet::Create(_scene, starsCamera->GetStarsScene(), controller);
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
    _mainSystem->OnUpdate();
    _generationSystem->OnUpdate();
    // _galaxySystem->OnUpdate();
    _levelSystem->OnUpdate();
    // _navigationSystem->OnUpdate();
    // _inventorySystem->OnUpdate();
    _shipSystem->OnUpdate();
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
