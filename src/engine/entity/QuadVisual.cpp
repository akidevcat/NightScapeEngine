#include "QuadVisual.h"
#include "../data/Mesh.h"
#include "../servers/RenderServer.h"

using namespace DirectX;

NSE::QuadVisual::QuadVisual()
{
    _mesh = RenderServer::Get()->GetPrimitiveQuadMesh();
}

NSE::QuadVisual::~QuadVisual()
{
    // ObjectServer::Get()->Destroy(_mesh);
}

void NSE::QuadVisual::RenderEntity(const NSE_Camera& camera)
{
    RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrix(camera->position), camera, GetUID());
}
