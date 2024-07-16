#include "TextVisual.h"

#include <sstream>

#include "../nsepch.h"
#include "../servers/AssetServer.h"
#include "../servers/RenderServer.h"

NSE_Shader NSE::TextVisual::_shader = nullptr;
NSE_Texture2D NSE::TextVisual::_defaultFontTexture = nullptr;

NSE::TextVisual::TextVisual()
{
    _mesh = RenderServer::Get()->GetPrimitiveQuadMesh();

    if (!_shader)
    {
        _shader = CreateObject<Shader>(L"Assets/Shaders/UIText.hlsl");
        _shader->Compile();
    }

    if (!_defaultFontTexture)
    {
        _defaultFontTexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/Default-FontAtlas.dds");
    }

    renderingMaterial = CreateObject<Material>(_shader);
    renderingMaterial->MakeTransparent();
    renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);

    SetFont(_defaultFontTexture, 4, 6);
}

NSE::TextVisual::~TextVisual()
{
    Release();
}

void NSE::TextVisual::SetFont(const NSE_Texture2D& fontTexture, int charSizeX, int charSizeY)
{
    _fontTexture = fontTexture;
    _fontCharSizeX = charSizeX;
    _fontCharSizeY = charSizeY;
}

void NSE::TextVisual::SetText(const std::string& text)
{
    auto textLength = text.length();
    auto flCreateBuffer = false;

    if (textLength == 0)
    {
        _textLength = 0;
        return;
    }

    if (!_textBuffer)
    {
        flCreateBuffer = true;
    }
    else if (_textBuffer->size() / 4 < textLength || _textBuffer->size() / 4 > textLength * 2)
    {
        Release();
        flCreateBuffer = true;
    }

    if (flCreateBuffer)
    {
        _textBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Default, 1, textLength, true, DXGI_FORMAT_R8_UINT);
    }

    auto bufferPtr = _textBuffer->As<char>();
    memcpy(bufferPtr, text.c_str(), textLength * sizeof(char));

    _textLength = (int)textLength;
}

void NSE::TextVisual::RenderEntity(const NSE_Camera& camera)
{
    SPID(_TextImage);
    SPID(_TextImage_AtlasParams);
    SPID(_TextBuffer);
    SPID(_TextLength);
    SPID(_Tint);
    SPID(_IsScreenSpace);

    if (_textLength <= 0)
        return;

    assert(_fontTexture);

    _textBuffer->Upload();

    if (renderingMaterial)
    {
        renderingMaterial->SetTexture(PID_TextImage, _fontTexture);
        renderingMaterial->SetVector(PID_TextImage_AtlasParams, {(float)_fontCharSizeX, (float)_fontCharSizeY, (float)_fontTexture->Width(), (float)_fontTexture->Height()});
        renderingMaterial->SetBuffer(PID_TextBuffer, _textBuffer);
        renderingMaterial->SetUnsignedInt(PID_TextLength, _textLength);
        renderingMaterial->SetColor(PID_Tint, color);
        renderingMaterial->SetFloat(PID_IsScreenSpace, isScreenSpace ? 1.0f : 0.0f);
    }

    if (isScreenSpace)
        RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrixUI(), camera, GetUID());
    else
        RenderServer::Get()->DrawMesh(_mesh, renderingMaterial, GetModelMatrix(camera->position), camera, GetUID());
}

void NSE::TextVisual::Release()
{
    DestroyObject(_textBuffer);
    _textLength = 0;
}
