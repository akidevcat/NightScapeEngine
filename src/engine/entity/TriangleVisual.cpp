#include "TriangleVisual.h"

TriangleVisual::TriangleVisual(ID3D11Device* device)
{
    _mesh = new Mesh{3, 3};

    _mesh->vertices[0] = VertexData{ XMFLOAT3(0, 1, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(0.5, 1), XMFLOAT4(1, 1, 1, 1) };
    _mesh->vertices[1] = VertexData{ XMFLOAT3(1, -1, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(1, 0), XMFLOAT4(1, 1, 1, 1) };
    _mesh->vertices[2] = VertexData{ XMFLOAT3(-1, -1, 0), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, 1) };

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
    if (!renderingMaterial)
    {
        return; // ToDo use missing mat instead
    }

    float t = time->Time();


    renderingMaterial->GetShader()->GetPixelShader()->SetDrawVar(ShaderUtils::PropertyToID("Time"), &t, sizeof(t));
    // renderingMaterial->GetShader()->GetPixelShader()->UploadDrawBuffer(render->GetDeviceContext());

    // render->GetDeviceContext()->PSSetConstantBuffers(0, 1, &renderingMaterial->GetShader()->GetPixelShader()->GetDrawProps()->bPtr);
    // Set the sampler state in the pixel shader.
    // render->GetDeviceContext()->PSSetSamplers(0, 1, &m_sampleState);

    render->DrawMesh(_mesh, renderingMaterial, XMMATRIX(), nullptr); // ToDo
}
