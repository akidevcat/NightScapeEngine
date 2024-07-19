#include "SceneEntity.h"

#include "../scene/Scene.h"
#include "../servers/SceneServer.h"
#include "../servers/ScreenServer.h"

using namespace DirectX;

NSE::SceneEntity::~SceneEntity()
{
}

XMMATRIX NSE::SceneEntity::GetModelMatrix(Vector3d relativeTo, float scaling) const
{
    auto positionCS = (XMFLOAT3)((position - relativeTo) * scaling);
    auto sc = scale;

    sc *= scaling;

    auto s = XMLoadFloat3(&sc);
    auto p = XMLoadFloat3(&positionCS);

    return XMMatrixAffineTransformation(s, g_XMZero, rotation, p);
}

XMMATRIX NSE::SceneEntity::GetModelMatrixUI() const
{
    auto positionUI = (XMFLOAT3)(position);
    positionUI.y *= - 1.0f;
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

NSE::Scene* NSE::SceneEntity::GetScene() const
{
    size_t sceneUid;
    if (GetSceneUID(sceneUid))
        return SceneServer::Get()->GetScene(sceneUid);

    return nullptr;
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
    if (_enabled == state)
    {
        return;
    }

    _enabled = state;

    if (_enabled)
        OnEnabled();
    else
        OnDisabled();
}

void NSE::SceneEntity::SetPositionUI(int posX, int posY, int resX, int resY)
{
    float x = (float)posX / (float)resX;
    float y = (float)posY / (float)resY;

    x = x * 2.0f - 1.0f;
    y = y * 2.0f - 1.0f;

    position.x = x;
    position.y = y;
}

void NSE::SceneEntity::AddPositionUI(int posX, int posY, int resX, int resY)
{
    float x = (float)posX / (float)resX;
    float y = (float)posY / (float)resY;

    x = x * 2.0f;
    y = y * 2.0f;

    position.x += x;
    position.y += y;
}
