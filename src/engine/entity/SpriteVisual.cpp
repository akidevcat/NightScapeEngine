#include "SpriteVisual.h"

#include "../servers/RenderServer.h"

NSE_Shader NSE::SpriteVisual::_shader = nullptr;

NSE::SpriteVisual::SpriteVisual()
{
    _mesh = RenderServer::Get()->GetPrimitiveQuadMesh();

    if (!_shader)
    {
        _shader = CreateObject<Shader>(L"Assets/Shaders/UIElement.hlsl");
        _shader->Compile();
    }

    renderingMaterial = CreateObject<Material>(_shader);
    renderingMaterial->MakeTransparent();
    renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
}

NSE::SpriteVisual::~SpriteVisual()
{
}

void NSE::SpriteVisual::RenderEntity(const NSE_Camera& camera)
{
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
        renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_IsPixelPerfect"), isPixelPerfect);
        renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_IsScreenSpace"), isScreenSpace);
        renderingMaterial->SetFloat2(ShaderUtils::PropertyToID("_Size"), float2(sizeX, sizeY));
        renderingMaterial->SetColor(ShaderUtils::PropertyToID("_Tint"), color);
        renderingMaterial->SetVector(ShaderUtils::PropertyToID("_Image_AtlasParams"), {sprite.xMin,sprite.yMin,rSizeX,rSizeY});
        renderingMaterial->SetTexture(ShaderUtils::PropertyToID("_Image"), sprite.atlasTexture);
    }

    if (isScreenSpace)
        RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrixUI(), camera, GetUID());
    else
        RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrix(camera->position), camera, GetUID());
}
