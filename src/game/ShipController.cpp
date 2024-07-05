#include "ShipController.h"

#include <algorithm>
#include <iostream>
#include <ostream>

#include "../engine/entity/QuadVisual.h"
#include "../engine/servers/AssetServer.h"
#include "../engine/servers/ObjectServer.h"
#include "../engine/servers/TimeServer.h"
#include "../engine/servers/InputServer.h"

using namespace DirectX;
using namespace NSE;

ShipController::ShipController(NSE::Scene* scene, float screenAspect)
{
    mesh = NSE::AssetsServer::Get()->LoadMeshAsset("Assets/Models/Cockpit.obj");
    auto renderingShader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/Base.hlsl");
    renderingShader->Compile();
    renderingMaterial = NSE::CreateObject<NSE::Material>(renderingShader);

    auto tex = NSE::AssetsServer::Get()->LoadTextureAsset(L"Assets/Models/T_Cockpit_Diffuse.dds");
    renderingMaterial->SetTexture(NSE::ShaderUtils::PropertyToID("_MainTex"), tex);

    auto testQuad = scene->Create<NSE::QuadVisual>();
    testQuad->renderingMaterial = renderingMaterial;
    testQuad->position = {0, 0, 10};

    _camera = scene->Create<NSE::Camera>();
    _camera->targetScene = scene;
    _camera->SetParams(screenAspect, 60.0f, 0.1f, 1000.0f, false, 0.0f);
    _camera->position = {0, 0, -1};
    _camera->clearMode = NSE::CAMERA_CLEAR_MODE_DEPTH;

    _cockpitLight = scene->Create<NSE::Light>();
    _cockpitLight->lightColor = {0.87f, 0.27f, 0.02f, 0.0f};
    _cockpitLight->lightIntensity = 0.2;

    _shipRadar = scene->Create<ShipRadarController>(scene);
    _shipRadar->scale = {0.6f, 0.6f, 0.6f};

    auto* barVertices = new float3[3];
    barVertices[0] = {-0.4, 0.2 - 0.03, 0};
    barVertices[1] = {-0.4, 0 - 0.03, 0};
    barVertices[2] = {-0.4, -0.2 - 0.03, 0};
    _fuelBar = scene->Create<ProgressBarVisual>(barVertices, 3);
    _fuelBar->foregroundColor = {0.04, 0.45, 1.0, 1.0f};
    _fuelBar->backgroundColor = {0.04 / 4.0f, 0.45 / 4.0f, 1.0 / 4.0f, 0.1f};
    _fuelBar->renderingMaterial->MakeTransparent();

    barVertices[0] = {0.4, 0.2 - 0.03, 0};
    barVertices[1] = {0.4, 0 - 0.03, 0};
    barVertices[2] = {0.4, -0.2 - 0.03, 0};
    _integrityBar = scene->Create<ProgressBarVisual>(barVertices, 3);
    _integrityBar->foregroundColor = {0.04, 1.0, 0.45, 1.0f};
    _integrityBar->backgroundColor = {0.04 / 4.0f, 1.0 / 4.0f, 0.45 / 4.0f, 0.1f};
    _integrityBar->renderingMaterial->MakeTransparent();
    _integrityBar->progress = 0.1;

    _crosshair = scene->Create<SpriteVisual>();
    _crosshair->sprite.atlasTexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Atlas.dds");
    _crosshair->sprite.SetRectRectPixel(8, 0, 4, 4);
    _crosshair->isPixelPerfect = true;
    _crosshair->isScreenSpace = false;
    _crosshair->color = {0.9, 0.3, 0.0, 0.1};
    _crosshair->position = {0.0, 0.0, 0.1};
    _crosshair->renderingMaterial->MakeTransparent();
}

ShipController::~ShipController()
{
    NSE::ObjectServer::Get()->Destroy(mesh);
}

void ShipController::OnUpdate()
{
    auto input = NSE::InputServer::Get();
    auto time = NSE::TimeServer::Get();

    NSE::Vector3d targetVelocity{};

    if (input->GetKey(DIK_W))
    {
        if (input->GetKey(DIK_LSHIFT))
        {
            targetVelocity += Forward() * 50.0f;
        }
        else
        {
            targetVelocity += Forward() * 20.0f;
        }
    }
    if (input->GetKey(DIK_S))
    {
        targetVelocity += Forward() * -10.0f;
    }
    if (input->GetKey(DIK_D))
    {
        targetVelocity += Right() * 10.0f;
    }
    if (input->GetKey(DIK_A))
    {
        targetVelocity += Right() * -10.0f;
    }
    if (input->GetKey(DIK_SPACE))
    {
        targetVelocity += Up() * 10.0f;
    }
    if (input->GetKey(DIK_LCONTROL))
    {
        targetVelocity += Up() * -10.0f;
    }
    if (input->GetKey(DIK_R))
    {
        // Make an artificial lag spike
        float val = 45714298.94127f;
        for (int i = 0; i < 10000000; i++)
        {
            val = sqrtf(val);
        }
    }

    _shipVelocity = NSE::Vector3d::Lerp(_shipVelocity, targetVelocity, time->Delta());

    if (input->GetKey(DIK_E))
    {
        _camMomentumR = _camMomentumR + (-4.0f - _camMomentumR) * time->Delta() * 4.0f;
    }
    if (input->GetKey(DIK_Q))
    {
        _camMomentumR = _camMomentumR + (4.0f - _camMomentumR) * time->Delta() * 4.0f;
    }

    _shipVelocity *= std::clamp(1.0 - time->Delta() * 0.5, 0.0, 1.0);
    _camMomentumR *= (float)std::clamp(1.0 - time->Delta() * 4.0, 0.0, 1.0);

    position += _shipVelocity * time->Delta();

    int dx, dy;
    input->GetMouseDelta(dy, dx);

    _camMomentumX += (float)dx / 1000.0f;
    _camMomentumY += (float)dy / 1000.0f;

    _camMomentumX *= powf(0.0001f, time->Delta());
    _camMomentumY *= powf(0.0001f, time->Delta());

    auto rot = XMQuaternionRotationRollPitchYaw(_camMomentumX * time->Delta(), _camMomentumY * time->Delta(), _camMomentumR * time->Delta());
    rotation = XMQuaternionMultiply(rot, rotation);

    // _camera->position = Vector3d::Lerp(_camera->position, position + Forward() * 1.0f + Up() * 0.1f, saturate(time->Delta() * 10.0f));
    _camera->position = position + Forward() * 1.0f + Up() * 0.1f;
    // ToDo noise

    _camera->rotation = XMQuaternionSlerp(_camera->rotation, rotation, saturate(30.0f * time->Delta()));


    _shipRadar->position = position + Forward() * 2.8f - Up() * 0.5f;
    _shipRadar->rotation = rotation;
    _shipRadar->GetParticleSystem()->position = _shipRadar->position;
    _shipRadar->GetParticleSystem()->rotation = _shipRadar->rotation;
    _fuelBar->position = _shipRadar->position;
    _fuelBar->rotation = _shipRadar->rotation;
    _integrityBar->position = _shipRadar->position;
    _integrityBar->rotation = _shipRadar->rotation;
    _cockpitLight->position = _shipRadar->position;

    _crosshair->position = position + Forward() * 3.0f;

    float velDot = (float)Vector3d::Dot(_shipVelocity.Normalized(), Forward());

    _camera->SetFov(60.0f + velDot * 2.0f * (1.0f - 1.0f / (1 + (float)_shipVelocity.Magnitude() * 0.2f)));
}
