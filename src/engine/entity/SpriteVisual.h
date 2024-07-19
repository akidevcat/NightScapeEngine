#ifndef SPRITEVISUAL_H
#define SPRITEVISUAL_H

#include "Camera.h"
#include "VisualEntity.h"
#include "../data/Mesh.h"
#include "../data/SpriteTexture.h"
#include "../render/Texture2D.h"

namespace NSE
{
    class SpriteVisual : public VisualEntity
    {
    public:
        enum class Alignment
        {
            TopLeft, TopCenter, TopRight,
            CenterLeft, Center, CenterRight,
            BottomLeft, BottomCenter, BottomRight
        };

    public:
        explicit SpriteVisual();
        ~SpriteVisual() override;

        void RenderEntity(const NSE_Camera& camera) override;
        void SetAlignment(Alignment alignment);

    public:
        SpriteTexture sprite{};
        bool isScreenSpace = false;
        float4 color{1,1,1,1};
        float2 alignmentOffset{0, 0};

    private:
        NSE_Mesh _mesh = nullptr;
        static NSE_Shader _shader;
    };


}

#endif //SPRITEVISUAL_H
