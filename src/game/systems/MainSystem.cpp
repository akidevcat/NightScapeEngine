#include "MainSystem.h"

#include "ShipSystem.h"
#include "../../engine/nsepch.h"
#include "../../engine/servers/SceneServer.h"
#include "../StarsCamera.h"

using namespace NSE;

MainSystem::MainSystem(Engine* engine) : _engine(engine)
{
    SPID(_MainTex);

    SceneServer::Get()->CreateScene(_scaledScene);
    SceneServer::Get()->CreateScene(_mainScene);
    SceneServer::Get()->CreateScene(_displayScene);

    _scaledCamera = _scaledScene->Create<Camera>();
    _scaledCamera->priority = -100;
    _scaledCamera->targetScene = _scaledScene;

    _displayCamera = _displayScene->Create<Camera>();
    _displayCamera->priority = 100;
    _displayCamera->targetScene = _displayScene;

    auto displayScreenShader = CreateObject<Shader>(L"Assets/Shaders/Display.hlsl");
    displayScreenShader->Compile();

    _displayScreenMaterial = CreateObject<Material>(displayScreenShader);

    _displayScreen = _displayScene->Create<QuadVisual>();
    _displayScreen->renderingMaterial = _displayScreenMaterial;

    _mainColorRenderTarget = CreateObject<RenderTexture>(120, 90, DXGI_FORMAT_R8G8B8A8_UNORM, false);
    _mainDepthRenderTarget = CreateObject<RenderTexture>(120, 90, DXGI_FORMAT_D24_UNORM_S8_UINT, true);

    _displayScreenMaterial->GetPSInputs()->SetResource(PID_MainTex, _mainColorRenderTarget->ResourceView());

    _scaledCamera->colorTarget = _mainColorRenderTarget;
    _scaledCamera->depthTarget = _mainDepthRenderTarget;
    _scaledCamera->clearMode = CAMERA_CLEAR_MODE_COLOR;
    _scaledCamera->clearColor = {0, 0, 0, 1};

    _skyboxQuad = _scaledScene->Create<QuadVisual>();
    auto skyboxShader = NSE::CreateObject<Shader>(L"Assets/Shaders/Skybox.hlsl");
    skyboxShader->Compile();
    auto skyboxMaterial = NSE::CreateObject<Material>(skyboxShader);
    skyboxMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    skyboxMaterial->renderQueue = Material::RENDER_QUEUE_BACKGROUND;
    _skyboxQuad->renderingMaterial = skyboxMaterial;

    _skyboxParticles = _scaledScene->Create<SkyboxStarsParticles>();
}

MainSystem::~MainSystem()
{

}

void MainSystem::OnStart()
{

}

void MainSystem::OnUpdate()
{
    auto mainCamera = ShipSystem::Get()->GetShipController()->GetCamera();

    if (!mainCamera)
    {
        return;
    }

    float aspect;
    float fov;
    float n, f;

    mainCamera->GetParams(&aspect, &fov, &n, &f, nullptr);
    // _scaledCamera->SetParams(aspect, fov, n, f, false, 0);
    _scaledCamera->SetParams(aspect, fov, 0.01f, 300.0f, false, 0);

    _scaledCamera->position = mainCamera->position;
    _scaledCamera->rotation = mainCamera->rotation;

    _skyboxParticles->position = mainCamera->position;

    // _particles->velocity = (float3)_shipController->GetShipVelocity();
    // _starsParticles->position = position;
}
