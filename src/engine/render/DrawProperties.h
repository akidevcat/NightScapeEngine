#ifndef DRAWPROPERTIES_H
#define DRAWPROPERTIES_H

#include <directxmath.h>

struct DrawProperties
{
    DirectX::XMMATRIX ProjectionMatrix;
    DirectX::XMMATRIX ModelMatrix;
    DirectX::XMMATRIX ViewMatrix;
    DirectX::XMMATRIX MVPMatrix;
};

#endif //DRAWPROPERTIES_H
