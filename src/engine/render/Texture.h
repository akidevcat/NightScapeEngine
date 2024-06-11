#ifndef TEXTURE_H
#define TEXTURE_H

#include "../entity/Object.h"
#include <d3d11.h>
#include <dxgiformat.h>

#define NSE_Texture obj_ptr<NSE::Texture>

namespace NSE
{
    class Texture : public Object
    {
    public:
        Texture() = delete;
        Texture(int width, int height);
        ~Texture() override;

        virtual void Release();

        [[nodiscard]] ID3D11Resource* Resource() const { return _d3dResource; }
        [[nodiscard]] ID3D11ShaderResourceView* ResourceView() const { return _d3dTextureView; }
        [[nodiscard]] size_t ByteSize() const { return _byteSize; }
        [[nodiscard]] int Width() const { return _width; }
        [[nodiscard]] int Height() const { return _height; }
        [[nodiscard]] bool HasMipmaps() const { return _useMipmaps; }
        [[nodiscard]] DXGI_FORMAT Format() const { return _format; }
        // Upload?
        // Release?
        // Reset?

    protected:
        void* _data = nullptr;
        size_t _byteSize = 0;
        int _width = 1;
        int _height = 1;
        bool _useMipmaps = false;
        ID3D11Resource* _d3dResource = nullptr;
        ID3D11ShaderResourceView* _d3dTextureView = nullptr;
        DXGI_FORMAT _format = DXGI_FORMAT_UNKNOWN;
    };
}



#endif //TEXTURE_H
