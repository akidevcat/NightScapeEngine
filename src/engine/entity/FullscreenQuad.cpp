#include "FullscreenQuad.h"

FullscreenQuad::FullscreenQuad(ID3D11Device *device)
{
    _mesh = new Mesh{4, 2 * 3};

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

    _mesh->UploadData(device);
}

FullscreenQuad::~FullscreenQuad()
{
    delete _mesh;
}

void FullscreenQuad::RenderEntity(RenderServer *render, TimeServer *time)
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
