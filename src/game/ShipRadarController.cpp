#include "ShipRadarController.h"

ShipRadarController::ShipRadarController()
{
    mesh = NSE::AssetsServer::Get()->LoadMeshAsset("Assets/Models/Icosphere.obj");
    auto shader = NSE::ObjectServer::Get()->Create<NSE::Shader>(L"Assets/Shaders/Radar.hlsl");
    shader->Compile();
    renderingMaterial = NSE::ObjectServer::Get()->Create<NSE::Material>(shader);
    renderingMaterial->MakeAdditive();
}

ShipRadarController::~ShipRadarController()
{
    DestroyObject(mesh);
}

void ShipRadarController::OnUpdate()
{

}