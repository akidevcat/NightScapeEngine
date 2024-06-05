#include "QuadVisual.h"
#include "../data/Mesh.h"
#include "../servers/RenderServer.h"

using namespace DirectX;

NSE::QuadVisual::QuadVisual()
{
    _mesh = CreateObject<Mesh>(4, 2 * 3);

    _mesh->vertices[0] = VertexData{XMFLOAT3(-1, -1, 0), XMFLOAT3(0, 0, 1), XMFLOAT2(0, 0)};
    _mesh->vertices[1] = VertexData{XMFLOAT3(-1, 1, 0), XMFLOAT3(0, 0, 1), XMFLOAT2(0, 1)};
    _mesh->vertices[2] = VertexData{XMFLOAT3(1, 1, 0), XMFLOAT3(0, 0, 1), XMFLOAT2(1, 1)};
    _mesh->vertices[3] = VertexData{XMFLOAT3(1, -1, 0), XMFLOAT3(0, 0, 1), XMFLOAT2(1, 0)};

    _mesh->indices[0] = 0;
    _mesh->indices[1] = 1;
    _mesh->indices[2] = 2;
    _mesh->indices[3] = 2;
    _mesh->indices[4] = 3;
    _mesh->indices[5] = 0;

    _mesh->Upload();
}

NSE::QuadVisual::~QuadVisual()
{
    ObjectServer::Get()->Destroy(_mesh);
}

void NSE::QuadVisual::RenderEntity(const NSE_Camera& camera)
{
    RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrix(camera->position), camera, GetUID());
}
