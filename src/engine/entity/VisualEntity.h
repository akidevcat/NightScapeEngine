#ifndef VISUALENTITY_H
#define VISUALENTITY_H

// #include "Camera.h"
#include "SceneEntity.h"
#include "../render/Material.h"

#define NSE_VisualEntity obj_ptr<NSE::VisualEntity>

namespace NSE
{
    class Camera;

    class VisualEntity : public SceneEntity
    {
    public:
        VisualEntity() = default;
        ~VisualEntity() override = 0;

        virtual void RenderEntity(const obj_ptr<Camera>& camera) = 0;

    public:
        NSE_Material renderingMaterial = nullptr;
        // XMVECTOR aabb;
    };
}

#endif //VISUALENTITY_H
