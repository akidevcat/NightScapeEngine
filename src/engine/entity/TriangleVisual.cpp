#include "TriangleVisual.h"

TriangleVisual::TriangleVisual(ID3D11Device* device)
{
    _mesh = new Mesh{3, 3};

    _mesh->vertices[0] = VertexData{XMFLOAT3(0, 1, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(0.5, 1)};
    _mesh->vertices[1] = VertexData{XMFLOAT3(1, -1, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(1, 0)};
    _mesh->vertices[2] = VertexData{XMFLOAT3(-1, -1, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0)};

    _mesh->indices[0] = 0;
    _mesh->indices[1] = 1;
    _mesh->indices[2] = 2;

    _mesh->UploadData(device);
}

TriangleVisual::~TriangleVisual()
{
    delete _mesh;
}

void TriangleVisual::RenderEntity(RenderServer *render, TimeServer* time)
{
    Material *material = nullptr;

    if (renderingMaterial)
    {
        float t = time->Time();
        // ToDo
        renderingMaterial->GetShader()->GetPixelShader()->SetDrawVar(ShaderUtils::PropertyToID("Time"), &t, sizeof(t));
        material = renderingMaterial;
    }

    render->DrawMesh(_mesh, material, XMMATRIX(), nullptr); // ToDo
}
