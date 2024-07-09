#ifndef TEXTVISUAL_H
#define TEXTVISUAL_H

#include "Camera.h"
#include "VisualEntity.h"
#include "../data/Mesh.h"

namespace NSE
{
    class TextVisual : public VisualEntity
    {
    public:
        explicit TextVisual();
        ~TextVisual() override;

        void SetFont(const NSE_Texture2D& fontTexture, int charSizeX, int charSizeY);
        void SetText(const std::string& text);

        void RenderEntity(const NSE_Camera& camera) override;

    private:
        void Release();

    public:
        bool isScreenSpace = false;
        float4 color{1,1,1,1};
    private:
        int _textLength = 0;
        std::string _text;
        NSE_Mesh _mesh = nullptr;
        NSE_GraphicsBuffer _textBuffer = nullptr;
        NSE_Texture2D _fontTexture = nullptr;
        int _fontCharSizeX = 0;
        int _fontCharSizeY = 0;
        static NSE_Shader _shader;
        static NSE_Texture2D _defaultFontTexture;
    };
}

#endif //TEXTVISUAL_H
