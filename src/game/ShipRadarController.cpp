#include "ShipRadarController.h"

ShipRadarController::ShipRadarController(NSE::Scene* scene)
{
    mesh = NSE::AssetsServer::Get()->LoadMeshAsset("Assets/Models/IcosphereSmooth3.obj");
    auto shader = NSE::ObjectServer::Get()->Create<NSE::Shader>(L"Assets/Shaders/Radar.hlsl");
    shader->Compile();
    renderingMaterial = NSE::ObjectServer::Get()->Create<NSE::Material>(shader);
    renderingMaterial->MakeTransparent();

    _pSystem = scene->Create<RadarParticleSystem>();
    AddTarget(_pSystem, {1, 0.7, 0.4, 1});
}

ShipRadarController::~ShipRadarController()
{
    DestroyObject(mesh);
}

void ShipRadarController::OnUpdate()
{

}

void ShipRadarController::AddTarget(const obj_ptr<NSE::SceneEntity> &target, float4 color)
{
    _pSystem->AddTarget(target, color);
}

void ShipRadarController::RemoveTarget(const obj_ptr<NSE::SceneEntity> &target)
{
    _pSystem->RemoveTarget(target);
}
