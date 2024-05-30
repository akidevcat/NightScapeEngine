#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <directxmath.h>

#include "Entity.h"
#include "../math/Math.h"

class SceneEntity : public Entity
{
public:
    DirectX::XMMATRIX GetModelMatrix(NSE::Vector3d relativeTo);
    DirectX::XMFLOAT3 Forward();

    bool GetSceneUID(size_t& uid);
    void SetSceneUID(size_t uid);
    void ResetSceneUID() { _sceneUID = 0; }

    virtual void OnUpdate() {}

    NSE::Vector3d     position = NSE::Vector3d{};
    DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
    DirectX::XMFLOAT3 scale    = DirectX::XMFLOAT3(1, 1, 1);

private:
    size_t            _sceneUID = 0;
};

#endif //SCENEENTITY_H
