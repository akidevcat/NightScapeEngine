#ifndef SPRITETEXTURE_H
#define SPRITETEXTURE_H

#include <utility>

#include "../render/Texture2D.h"

namespace NSE
{
    class SpriteTexture
    {
    public:
        SpriteTexture();
        explicit SpriteTexture(NSE_Texture2D atlasTexture);

    public:
        void SetRectRelative(const float xMin, const float yMin, const float xSize, const float ySize)
        {
            this->xMin = xMin;
            this->yMin = yMin;
            xMax = xMin + xSize;
            yMax = yMin + ySize;
        }

        void SetRectRectPixel(const int xMin, const int yMin, const int xSize, const int ySize)
        {
            assert(atlasTexture);
            const auto w = (float)(atlasTexture->Width() - 1);
            const auto h = (float)(atlasTexture->Height() - 1);
            this->xMin = (float)xMin / w;
            this->yMin = (float)yMin / h;
            xMax = (float)(xMin + xSize) / w;
            yMax = (float)(yMin + ySize) / h;
        }

    public:
        NSE_Texture2D atlasTexture = nullptr;
        float xMin = 0, yMin = 0;
        float xMax = 0, yMax = 0;
    };

    inline SpriteTexture::SpriteTexture() : atlasTexture(nullptr) {}

    inline SpriteTexture::SpriteTexture(NSE_Texture2D atlasTexture) : atlasTexture(std::move(atlasTexture)) {}
}

#endif //SPRITETEXTURE_H
