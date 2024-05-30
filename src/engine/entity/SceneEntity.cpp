#include "SceneEntity.h"

using namespace DirectX;

XMMATRIX SceneEntity::GetModelMatrix(NSE::Vector3d relativeTo)
{
    auto positionCS = (XMFLOAT3)(position - relativeTo);

    auto z = g_XMZero;
    auto s = XMLoadFloat3(&scale);
    auto p = XMLoadFloat3(&positionCS);
    auto m = XMMatrixAffineTransformation(s, z, rotation, p);
    auto d = XMMatrixDeterminant(m);

    return m;
    // return XMMatrixInverse(&d, m);

    // return XMMatrixTransformation(g_XMZero, g_XMIdentityR3, g_XMOne3, g_XMZero, g_XMIdentityR3, p);
}

XMFLOAT3 SceneEntity::Forward()
{
    return {}; // ToDo
}

bool SceneEntity::GetSceneUID(size_t &uid)
{
    if (_sceneUID == 0)
    {
        uid = 0;
        return false;
    }

    uid = _sceneUID - 1;
    return true;
}

void SceneEntity::SetSceneUID(size_t uid)
{
    _sceneUID = uid + 1;
}
