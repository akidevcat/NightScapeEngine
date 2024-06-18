#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Texture.h"
#include "../obj_ptr.h"

// #define NSE_Texture2D obj_ptr<NSE::Texture2D>

namespace NSE
{
    class Texture2D : public Texture
    {
    public:
        Texture2D() = delete;
        Texture2D(int width, int height, DXGI_FORMAT format);
        Texture2D(ID3D11Resource* resource, ID3D11ShaderResourceView* view);
        ~Texture2D() override;

        void Upload();
        void Release() override;
    };
}

typedef obj_ptr<NSE::Texture2D> NSE_Texture2D;

#endif //TEXTURE2D_H
