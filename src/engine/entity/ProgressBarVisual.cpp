#include "ProgressBarVisual.h"
#include "../nsepch.h"

#include "../servers/RenderServer.h"
#include "../servers/AssetServer.h"

NSE_Shader NSE::ProgressBarVisual::_shader = nullptr;

NSE::ProgressBarVisual::ProgressBarVisual()
{
    // mesh = CreateObject<Mesh>(pathLength, pathLength);
    // mesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
    //
    // const auto indices = mesh->GetIndices();
    // for (int i = 0; i < pathLength; i++)
    // {
    //     indices[i] = i;
    // }
    //
    // const auto vertices = mesh->GetVertices();
    // VertexData data;
    // for (int i = 0; i < pathLength; i++)
    // {
    //     data.position = path[i];
    //     data.normal = {};
    //     data.color = {};
    //     data.uv = {(float)i / (float)(pathLength - 1), 0.5f}; // ToDo make length-based
    //     vertices[i] = data;
    // }
    //
    // mesh->Upload();
    //
    // if (_shader == nullptr)
    // {
    //     _shader = CreateObject<Shader>(L"Assets/Shaders/ProgressBarLine.hlsl");
    //     _shader->Compile();
    // }
    //
    // renderingMaterial = CreateObject<Material>(_shader);

    _mesh = RenderServer::Get()->GetPrimitiveQuadMesh();

    if (!_shader)
    {
        _shader = CreateObject<Shader>(L"Assets/Shaders/UIProgressBar.hlsl");
        _shader->Compile();
    }

    renderingMaterial = CreateObject<Material>(_shader);
    renderingMaterial->MakeTransparent();
    renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);

    sprite.atlasTexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Atlas.dds");
    sprite.SetRectRectPixel(16, 0, 37, 11);
    isScreenSpace = false;
}

void NSE::ProgressBarVisual::RenderEntity(const obj_ptr<Camera> &camera)
{
    SPID(_ProgressValue);
    SPID(_ForegroundColor);
    SPID(_BackgroundColor);
    SPID(_IsScreenSpace);
    SPID(_Size);
    SPID(_Image_AtlasParams);
    SPID(_Image);
    SPID(_Tint);
    SPID(_UVOffset);

    if (sprite.atlasTexture == nullptr)
    {
        return;
    }

    if (renderingMaterial)
    {
        float rSizeX = (sprite.xMax - sprite.xMin);
        float rSizeY = (sprite.yMax - sprite.yMin);
        float sizeX = rSizeX * (float)sprite.atlasTexture->Width();
        float sizeY = rSizeY * (float)sprite.atlasTexture->Height();
        renderingMaterial->SetFloat(PID_IsScreenSpace, isScreenSpace);
        renderingMaterial->SetFloat2(PID_Size, float2(sizeX, sizeY));
        renderingMaterial->SetVector(PID_Image_AtlasParams, {sprite.xMin,sprite.yMin,rSizeX,rSizeY});
        renderingMaterial->SetTexture(PID_Image, sprite.atlasTexture);
        renderingMaterial->SetFloat(PID_ProgressValue, progress);
        renderingMaterial->SetFloat4(PID_ForegroundColor, foregroundColor);
        renderingMaterial->SetFloat4(PID_BackgroundColor, backgroundColor);
        renderingMaterial->SetFloat4(PID_Tint, color);
        renderingMaterial->SetFloat2(PID_UVOffset, float2{invertX ? -1.0f : 0.0f, invertY ? -1.0f : 0.0f});
    }

    if (isScreenSpace)
        RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrixUI(), camera, GetUID());
    else
        RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrix(camera->position), camera, GetUID());
}
