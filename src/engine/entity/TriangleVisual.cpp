#include "TriangleVisual.h"

using namespace DirectX;

TriangleVisual::TriangleVisual(ID3D11Device* device)
{
    _mesh = new Mesh{3, 3};

    _mesh->vertices[0] = VertexData{XMFLOAT3(0, 1.1547f, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(0.5, 1)};
    _mesh->vertices[1] = VertexData{XMFLOAT3(1.0f, -0.57735f, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(1, 0)};
    _mesh->vertices[2] = VertexData{XMFLOAT3(-1.0f, -0.57735f, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0)};

    _mesh->indices[0] = 0;
    _mesh->indices[1] = 1;
    _mesh->indices[2] = 2;

    _mesh->UploadData(device);
}

TriangleVisual::~TriangleVisual()
{
    delete _mesh;
}

void TriangleVisual::OnUpdate()
{

}

void TriangleVisual::RenderEntity(RenderServer *render, TimeServer* time, Camera *camera)
{
    render->DrawMesh(_mesh, renderingMaterial, GetModelMatrix(camera->position), camera); // ToDo
}
