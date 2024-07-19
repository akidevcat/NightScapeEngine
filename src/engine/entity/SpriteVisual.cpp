#include "SpriteVisual.h"

#include "../nsepch.h"
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
    SPID(_IsScreenSpace);
    SPID(_Size);
    SPID(_AlignmentOffset);
    SPID(_Tint);
    SPID(_Image_AtlasParams);
    SPID(_Image);

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
        sizeX = round(sizeX); // ToDo
        sizeY = round(sizeY);
        renderingMaterial->SetFloat(PID_IsScreenSpace, isScreenSpace);
        renderingMaterial->SetFloat2(PID_Size, float2(sizeX, sizeY));
        renderingMaterial->SetFloat2(PID_AlignmentOffset, alignmentOffset);
        renderingMaterial->SetColor(PID_Tint, color);
        renderingMaterial->SetVector(PID_Image_AtlasParams, {sprite.xMin,sprite.yMin,rSizeX,rSizeY});
        renderingMaterial->SetTexture(PID_Image, sprite.atlasTexture);
    }

    if (isScreenSpace)
        RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrixUI(), camera, GetUID());
    else
        RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrix(camera->position), camera, GetUID());
}

void NSE::SpriteVisual::SetAlignment(Alignment alignment)
{
    switch (alignment)
    {
        case Alignment::TopLeft:
            alignmentOffset = {-1, -1};
            break;
        case Alignment::TopCenter:
            alignmentOffset = {0, -1};
        break;
        case Alignment::TopRight:
            alignmentOffset = {1, -1};
        break;
        case Alignment::CenterLeft:
            alignmentOffset = {-1, 0};
        break;
        case Alignment::Center:
            alignmentOffset = {0, 0};
        break;
        case Alignment::CenterRight:
            alignmentOffset = {1, 0};
        break;
        case Alignment::BottomLeft:
            alignmentOffset = {-1, 1};
        break;
        case Alignment::BottomCenter:
            alignmentOffset = {0, 1};
        break;
        case Alignment::BottomRight:
            alignmentOffset = {1, 1};
        break;
    }
}
