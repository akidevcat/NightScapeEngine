#include "TriangleVisual.h"

#include "Camera.h"
#include "../servers/RenderServer.h"
#include "../servers/AssetServer.h"

using namespace DirectX;

NSE::TriangleVisual::TriangleVisual()
{
    _mesh = CreateObject<Mesh>(3, 3);

    _mesh->vertices[0] = VertexData{XMFLOAT3(0, 1.1547f, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(0.5, 1)};
    _mesh->vertices[1] = VertexData{XMFLOAT3(1.0f, -0.57735f, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(1, 0)};
    _mesh->vertices[2] = VertexData{XMFLOAT3(-1.0f, -0.57735f, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0)};

    _mesh->indices[0] = 0;
    _mesh->indices[1] = 1;
    _mesh->indices[2] = 2;

    _mesh->Upload();
}

NSE::TriangleVisual::~TriangleVisual()
{
    ObjectServer::Get()->Destroy(_mesh);
}

void NSE::TriangleVisual::OnUpdate()
{

}

void NSE::TriangleVisual::RenderEntity(const NSE_Camera& camera)
{
    RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrix(camera->position), camera, GetUID());
}
