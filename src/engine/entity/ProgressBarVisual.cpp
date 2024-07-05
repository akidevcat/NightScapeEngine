#include "ProgressBarVisual.h"
#include "../nsepch.h"

NSE_Shader NSE::ProgressBarVisual::_shader = nullptr;

NSE::ProgressBarVisual::ProgressBarVisual(const float3 *path, int pathLength)
{
    mesh = CreateObject<Mesh>(pathLength, pathLength);
    mesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

    const auto indices = mesh->GetIndices();
    for (int i = 0; i < pathLength; i++)
    {
        indices[i] = i;
    }

    const auto vertices = mesh->GetVertices();
    VertexData data;
    for (int i = 0; i < pathLength; i++)
    {
        data.position = path[i];
        data.normal = {};
        data.color = {};
        data.uv = {(float)i / (float)(pathLength - 1), 0.5f}; // ToDo make length-based
        vertices[i] = data;
    }

    mesh->Upload();

    if (_shader == nullptr)
    {
        _shader = CreateObject<Shader>(L"Assets/Shaders/ProgressBarLine.hlsl");
        _shader->Compile();
    }

    renderingMaterial = CreateObject<Material>(_shader);
}

void NSE::ProgressBarVisual::RenderEntity(const obj_ptr<Camera> &camera)
{
    SPID(_ProgressValue);
    SPID(_ForegroundColor);
    SPID(_BackgroundColor);

    if (renderingMaterial)
    {
        renderingMaterial->SetFloat(PID_ProgressValue, progress);
        renderingMaterial->SetFloat4(PID_ForegroundColor, foregroundColor);
        renderingMaterial->SetFloat4(PID_BackgroundColor, backgroundColor);
    }

    VisualMeshEntity::RenderEntity(camera);
}
