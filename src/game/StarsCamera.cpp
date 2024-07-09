#include "StarsCamera.h"

#include "planetary/Planet.h"
#include "../engine/servers/RenderServer.h"
#include "../engine/servers/SceneServer.h"

StarsCamera::StarsCamera(const NSE_Camera& parentCamera, const obj_ptr<ShipController>& controller)
{
    static size_t sid_PixelSizeX = NSE::ShaderUtils::PropertyToID("_PixelSizeX");
    static size_t sid_PixelSizeY = NSE::ShaderUtils::PropertyToID("_PixelSizeY");

    priority = -100;
    _parentCamera = parentCamera;
    NSE::SceneServer::Get()->CreateScene(_starsScene);
    this->targetScene = _starsScene;

    // this->targetRT = targetRT;
    this->colorTarget = parentCamera->colorTarget;
    this->depthTarget = parentCamera->depthTarget;

    this->clearMode = NSE::CAMERA_CLEAR_MODE_COLOR;
    this->clearColor = {0, 0, 0, 1};

    this->CopyParams(parentCamera);

    _quadShader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/StarQuad.hlsl");
    _quadShader->Compile();
    _quadMaterial = NSE::CreateObject<NSE::Material>(_quadShader);
    _quadMaterial->MakeAdditive();
    _quadMaterial->SetUnsignedInt(sid_PixelSizeX, 5*3);
    _quadMaterial->SetUnsignedInt(sid_PixelSizeY, 9*3);

    for (int i = 0; i < 100; i++)
    {
        auto quad = _starsScene->Create<NSE::QuadVisual>();

        auto rot = DirectX::XMQuaternionRotationRollPitchYaw(NSE::Math::Random() * 2.0f * DirectX::XM_PI, NSE::Math::Random() * 2.0f * DirectX::XM_PI, 0);
        quad->position = DirectX::XMMatrixRotationQuaternion(rot).r[2];
        quad->rotation = rot;
        // quad->position = DirectX::XMVector3Rotate({-1, 0, 0}, rot);
        quad->position *= 1000.0f;
        quad->scale = {1, 1, 1};

        // quad->position = {0, 0, 0.5};
        quad->renderingMaterial = _quadMaterial;
        _largeStars.emplace_back(quad);
    }

    _particles = _starsScene->Create<StarDustParticles>();

    _shipController = controller;
    // _particles->position += {0, 0, 1};

    // _starsScene->Create<Planet>();

    auto skyboxQuad = _starsScene->Create<NSE::QuadVisual>();
    auto skyboxShader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/Skybox.hlsl");
    skyboxShader->Compile();
    auto skyboxMaterial = NSE::CreateObject<NSE::Material>(skyboxShader);
    skyboxMaterial->SetDepthWrite(NSE::ShaderDepthState::Disabled);
    skyboxMaterial->renderQueue = NSE::Material::RenderQueue::Background;
    skyboxQuad->renderingMaterial = skyboxMaterial;
}

StarsCamera::~StarsCamera()
{
    // ToDo
    // NSE::SceneServer::Get()->UnloadScene(_starsScene->GetUID());

    for (const auto& quad : _largeStars)
    {
        if (quad) // ToDo why?
        _starsScene->Destroy(quad);
    }

    NSE::DestroyObject(_quadMaterial);
    NSE::DestroyObject(_quadShader);
}

void StarsCamera::OnUpdate()
{
    float aspect;
    float fov;
    float n, f;

    _parentCamera->GetParams(&aspect, &fov, &n, &f, nullptr);
    SetParams(aspect, fov, n, f, false, 0);

    rotation = _parentCamera->rotation;
    position = _parentCamera->position;

    _particles->velocity = (float3)_shipController->GetShipVelocity();
}
