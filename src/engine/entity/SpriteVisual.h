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
        explicit SpriteVisual();
        ~SpriteVisual() override;

        void RenderEntity(const NSE_Camera& camera) override;

    public:
        SpriteTexture sprite{};
        bool isPixelPerfect = true;
        bool isScreenSpace = false;
        float4 color{1,1,1,1};

    private:
        NSE_Mesh _mesh = nullptr;
        static NSE_Shader _shader;
    };


}

#endif //SPRITEVISUAL_H
