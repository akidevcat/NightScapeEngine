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

ShipController::ShipController(NSE::Scene* scene, float screenAspect)
{
    mesh = NSE::AssetsServer::Get()->LoadMeshAsset("Assets/Models/Cockpit.obj");
    auto renderingShader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/Base.hlsl");
    renderingShader->Compile();
    renderingMaterial = NSE::CreateObject<NSE::Material>(renderingShader);

    auto tex = NSE::AssetsServer::Get()->LoadTextureAsset(L"Assets/Models/test.dds");
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
    _cockpitLight->lightColor = {0.22f, 0.13f, 0.02f, 1.0f};
    _cockpitLight->lightIntensity = 6.0f;

    _shipRadar = scene->Create<ShipRadarController>();
    _shipRadar->scale = {0.45f, 0.45f, 0.45f};
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
    // _speedMomentum *= 0.9995f;
    position += _shipVelocity * time->Delta();

    int dx, dy;
    input->GetMouseDelta(dy, dx);

    _camMomentumX += (float)dx / 1000000.0f;
    _camMomentumY += (float)dy / 1000000.0f;

    _camMomentumX *= powf(0.0001f, time->Delta());
    _camMomentumY *= powf(0.0001f, time->Delta());

    auto rot = XMQuaternionRotationRollPitchYaw(_camMomentumX, _camMomentumY, _camMomentumR * time->Delta());
    rotation = XMQuaternionMultiply(rot, rotation);

    // _camera->position = position + Forward() * 1.0f + Up() * 0.1f;
    _camera->position = NSE::Vector3d::Lerp(_camera->position, position + Forward() * 1.0f + Up() * 0.1f, time->Delta() * 100.0f);
    // ToDo noise
    // _camera->rotation = rotation;

    _camera->rotation = XMQuaternionSlerp(_camera->rotation, rotation, 0.03f);
    // _camera->

    _shipRadar->position = position + Forward() * 2.4f - Up() * 0.35f;
    _cockpitLight->position = position + Forward() * 2.4f - Up() * 0.35f;

    float velDot = (float)NSE::Vector3d::Dot(_shipVelocity.Normalized(), Forward());

    _camera->SetFov(60.0 + velDot * 5.0 * (1.0 - 1.0 / (1 + _shipVelocity.Magnitude() * 0.2)));
}
