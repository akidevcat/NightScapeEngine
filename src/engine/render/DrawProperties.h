#ifndef DRAWPROPERTIES_H
#define DRAWPROPERTIES_H

#include <directxmath.h>

struct DrawProperties
{
    uint32_t ObjectID;
    DirectX::XMMATRIX ProjectionMatrix;
    DirectX::XMMATRIX ModelMatrix;
    DirectX::XMMATRIX ViewMatrix;
    DirectX::XMMATRIX MVPMatrix;
    uint32_t TargetResolutionX;
    uint32_t TargetResolutionY;
};

#endif //DRAWPROPERTIES_H
