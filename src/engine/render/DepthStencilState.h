#ifndef DEPTHSTENCILSTATE_H
#define DEPTHSTENCILSTATE_H
#include <d3d11.h>

#include "../entity/Object.h"

namespace NSE
{
    class DepthStencilState : public Object
    {
        DepthStencilState();
        explicit DepthStencilState(const D3D11_DEPTH_STENCIL_DESC& description);
        ~DepthStencilState() override;
        void Release();

        explicit operator ID3D11DepthStencilState*() const { return _d3dObject; }

    private:
        ID3D11DepthStencilState* _d3dObject = nullptr;
    };
}

#endif //DEPTHSTENCILSTATE_H
