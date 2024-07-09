#include "SceneEntity.h"

using namespace DirectX;

NSE::SceneEntity::~SceneEntity()
{
}

XMMATRIX NSE::SceneEntity::GetModelMatrix(Vector3d relativeTo, float scaling) const
{
    auto positionCS = (XMFLOAT3)((position - relativeTo) * scaling);
    auto sc = scale;

    // positionCS *= scaling;
    sc *= scaling;

    auto s = XMLoadFloat3(&sc);
    auto p = XMLoadFloat3(&positionCS);

    return XMMatrixAffineTransformation(s, g_XMZero, rotation, p);
}

XMMATRIX NSE::SceneEntity::GetModelMatrixUI() const
{
    auto positionUI = (XMFLOAT3)(position);
    auto p = XMLoadFloat3(&positionUI);

    return XMMatrixAffineTransformation(g_XMOne, g_XMZero, g_XMIdentityR3, p);
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

void NSE::SceneEntity::SetEnabled(bool state)
{
    _enabled = state;
}
