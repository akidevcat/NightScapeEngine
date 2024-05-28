#include "SceneEntity.h"

DirectX::XMMATRIX SceneEntity::GetModelMatrix(NSE::Vector3d relativeTo)
{
    auto positionCS = (DirectX::XMFLOAT3)(position - relativeTo);

    auto z = DirectX::g_XMZero;
    auto s = DirectX::XMLoadFloat3(&scale);
    auto p = DirectX::XMLoadFloat3(&positionCS);

    return DirectX::XMMatrixAffineTransformation(s, z, rotation, p);
}
