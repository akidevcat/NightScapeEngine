#ifndef DRAWPROPERTIES_H
#define DRAWPROPERTIES_H

#include <directxmath.h>

struct DrawProperties
{
    DirectX::XMMATRIX ProjectionMatrix;
    DirectX::XMMATRIX ModelMatrix;
};

#endif //DRAWPROPERTIES_H
