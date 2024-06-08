#include "ShipController.h"

#include <algorithm>

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
    _camera = scene->Create<NSE::Camera>();
    _camera->targetScene = scene;
    _camera->SetParams(screenAspect, 60.0f, 0.1f, 1000.0f, false, 0.0f);
    _camera->position = {0, 0, -1};
    _camera->clearMode = NSE::CAMERA_CLEAR_MODE_DEPTH;

    _shipRadar = scene->Create<ShipRadarController>();
    _shipRadar->scale = {0.3f, 0.3f, 0.3f};
}

ShipController::~ShipController()
{
    NSE::ObjectServer::Get()->Destroy(mesh);
}

void ShipController::OnUpdate()
{
    auto input = NSE::InputServer::Get();
    auto time = NSE::TimeServer::Get();

    if (input->GetKey(DIK_W))
    {
        _speedMomentum += time->Delta() * 8.0f;
        // position += forward * time->Delta() * speed;
    }
    if (input->GetKey(DIK_S))
    {
        _speedMomentum -= time->Delta() * 8.0f;
        // position += -forward * time->Delta() * speed;
    }
    // _speedMomentum = std::clamp(_speedMomentum, -1.0f, 3.0f);

    _speedMomentum *= 0.9995f;
    position += Forward() * _speedMomentum * time->Delta();

    int dx, dy;
    input->GetMouseDelta(dy, dx);

    _camMomentumX += (float)dx / 1000000.0f;
    _camMomentumY += (float)dy / 1000000.0f;

    _camMomentumX *= powf(0.0001f, time->Delta());
    _camMomentumY *= powf(0.0001f, time->Delta());

    auto rot = XMQuaternionRotationRollPitchYaw(_camMomentumX, _camMomentumY, 0);
    rotation = XMQuaternionMultiply(rot, rotation);

    _camera->position = position + Forward() * 1.0f + Up() * 0.1f;
    // _camera->rotation = rotation;

    _camera->rotation = XMQuaternionSlerp(_camera->rotation, rotation, 0.03f);

    _shipRadar->position = position + Forward() * 2.0f - Up() * 0.29f;

}
