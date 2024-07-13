#include "StarsCamera.h"

#include "planetary/Planet.h"
#include "../engine/servers/RenderServer.h"
#include "../engine/servers/SceneServer.h"

StarsCamera::StarsCamera(const NSE_Camera& parentCamera, const obj_ptr<ShipController>& controller)
{
    // priority = -100;
    // _parentCamera = parentCamera;
    // NSE::SceneServer::Get()->CreateScene(_starsScene);
    // this->targetScene = _starsScene;

    // this->colorTarget = parentCamera->colorTarget;
    // this->depthTarget = parentCamera->depthTarget;

    // this->clearMode = NSE::CAMERA_CLEAR_MODE_COLOR;
    // this->clearColor = {0, 0, 0, 1};

    // this->CopyParams(parentCamera);

    // _particles = _starsScene->Create<StarDustParticles>();
    // _starsParticles = _starsScene->Create<SkyboxStarsParticles>();

    // _shipController = controller;

    // auto skyboxQuad = _starsScene->Create<NSE::QuadVisual>();
    // auto skyboxShader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/Skybox.hlsl");
    // skyboxShader->Compile();
    // auto skyboxMaterial = NSE::CreateObject<NSE::Material>(skyboxShader);
    // skyboxMaterial->SetDepthWrite(NSE::ShaderDepthState::Disabled);
    // skyboxMaterial->renderQueue = NSE::Material::RENDER_QUEUE_BACKGROUND;
    // skyboxQuad->renderingMaterial = skyboxMaterial;
}

StarsCamera::~StarsCamera()
{

}

void StarsCamera::OnUpdate()
{
    // float aspect;
    // float fov;
    // float n, f;
    //
    // _parentCamera->GetParams(&aspect, &fov, &n, &f, nullptr);
    // SetParams(aspect, fov, n, f, false, 0);
    //
    // rotation = _parentCamera->rotation;
    // position = _parentCamera->position;
    //
    // _particles->velocity = (float3)_shipController->GetShipVelocity();
    // _starsParticles->position = position;
}
