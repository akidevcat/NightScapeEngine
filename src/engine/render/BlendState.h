#ifndef BLENDSTATE_H
#define BLENDSTATE_H

#include <dxgi.h>
#include <d3d11.h>
#include <directxmath.h>

#include "../entity/Object.h"

#define NSE_BlendState obj_ptr<NSE::BlendState>

namespace NSE
{
    class BlendState : public Object
    {
    public:
        BlendState();
        explicit BlendState(const D3D11_BLEND_DESC& description);
        ~BlendState() override;
        void Release();

        explicit operator ID3D11BlendState*() const { return _d3dObject; }

    private:
        ID3D11BlendState* _d3dObject = nullptr;
    };
}

#endif //BLENDSTATE_H
