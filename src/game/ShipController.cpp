#include "ShipController.h"
#include "../engine/servers/AssetServer.h"
#include "../engine/servers/ObjectServer.h"

ShipController::ShipController()
{
    mesh = NSE::AssetsServer::Get()->LoadMeshAsset("Assets/Models/Cockpit.obj");
}

ShipController::~ShipController()
{
    NSE::ObjectServer::Get()->Destroy(mesh);
}
