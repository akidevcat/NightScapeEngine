#include "SceneEntity.h"

using namespace DirectX;

NSE::SceneEntity::~SceneEntity()
{
}

XMMATRIX NSE::SceneEntity::GetModelMatrix(Vector3d relativeTo) const
{
    auto positionCS = (XMFLOAT3)(position - relativeTo);

    auto s = XMLoadFloat3(&scale); // ToDo
    auto p = XMLoadFloat3(&positionCS);

    return XMMatrixAffineTransformation(s, g_XMZero, rotation, p);
}

XMVECTOR NSE::SceneEntity::Forward() const
{
    return XMVector4Transform({0, 0, 1, 0}, GetModelMatrix({}));
}

XMVECTOR NSE::SceneEntity::Right() const
{
    return XMVector4Transform({1, 0, 0, 0}, GetModelMatrix({}));
}

XMVECTOR NSE::SceneEntity::Up() const
{
    return XMVector4Transform({0, 1, 0, 0}, GetModelMatrix({}));
}

bool NSE::SceneEntity::GetSceneUID(size_t &uid) const
{
    if (_sceneUID == 0)
    {
        uid = -1; // ToDo
        return false;
    }

    uid = _sceneUID - 1;
    return true;
}

void NSE::SceneEntity::SetSceneUID(size_t uid)
{
    _sceneUID = uid + 1;
}
