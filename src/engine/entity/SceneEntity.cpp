#include "SceneEntity.h"

using namespace DirectX;

XMMATRIX SceneEntity::GetModelMatrix(NSE::Vector3d relativeTo)
{
    auto positionCS = (XMFLOAT3)(position - relativeTo);

    auto z = g_XMZero;
    auto s = XMLoadFloat3(&scale);
    auto p = XMLoadFloat3(&positionCS);

    return XMMatrixAffineTransformation(s, z, rotation, p);
    // return XMMatrixTransformation(g_XMZero, g_XMIdentityR3, g_XMOne3, g_XMZero, g_XMIdentityR3, p);
}

XMFLOAT3 SceneEntity::Forward()
{
    return {}; // ToDo
}
