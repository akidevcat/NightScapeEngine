#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <directxmath.h>

#include "Entity.h"
#include "../math/Math.h"

class SceneEntity : public Entity
{
public:
    DirectX::XMMATRIX GetModelMatrix(NSE::Vector3d relativeTo);

    virtual void OnUpdate() {}

    NSE::Vector3d     position = NSE::Vector3d{};
    // DirectX::XMFLOAT3 position = {};
    DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
    DirectX::XMFLOAT3 scale    = DirectX::XMFLOAT3(1, 1, 1);
};

#endif //SCENEENTITY_H
