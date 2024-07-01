#include "Planet.h"

#include "../engine/servers/AssetServer.h"

using namespace NSE;

void Planet::OnCreated()
{
    mesh = AssetsServer::Get()->LoadMeshAsset("Assets/Models/IcosphereSmooth3.obj");
    auto shader = CreateObject<Shader>(L"Assets/Shaders/Planet.hlsl");
    shader->Compile();
    renderingMaterial = CreateObject<Material>(shader);

    position += {0, 0, 2};
}
