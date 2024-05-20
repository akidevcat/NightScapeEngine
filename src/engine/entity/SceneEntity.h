#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <directxmath.h>

#include "Entity.h"

using namespace DirectX;

class SceneEntity : public Entity
{
public:
    XMFLOAT3 position = XMFLOAT3(0, 0, 0);
    XMFLOAT4 rotation = XMFLOAT4(0, 0, 0, 1);
    XMFLOAT3 scale    = XMFLOAT3(1, 1, 1);

public:
    virtual void OnUpdate() {}
};

#endif //SCENEENTITY_H
