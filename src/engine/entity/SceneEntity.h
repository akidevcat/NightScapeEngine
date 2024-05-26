#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <directxmath.h>

#include "Entity.h"

class SceneEntity : public Entity
{
public:
    DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0);
    DirectX::XMFLOAT4 rotation = DirectX::XMFLOAT4(0, 0, 0, 1);
    DirectX::XMFLOAT3 scale    = DirectX::XMFLOAT3(1, 1, 1);

public:
    virtual void OnUpdate() {}
};

#endif //SCENEENTITY_H
