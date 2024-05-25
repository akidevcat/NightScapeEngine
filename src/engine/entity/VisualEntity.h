#ifndef VISUALENTITY_H
#define VISUALENTITY_H

#include "SceneEntity.h"
#include "../render/Material.h"
#include "../servers/RenderServer.h"
#include "../servers/TimeServer.h"

class VisualEntity : public SceneEntity
{
public:
    VisualEntity();
    ~VisualEntity();

    virtual void RenderEntity(RenderServer* render, TimeServer* time) = 0;
    // ToDo GetAABB, change to local aabb...
    // ToDo Material
public:
    Material* renderingMaterial = nullptr;
    // XMVECTOR aabb;
};

#endif //VISUALENTITY_H
