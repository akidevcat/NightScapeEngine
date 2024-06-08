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

        explicit operator int64_t() const
        {
            if (!renderingMaterial)
            {
                return 0;
            }

            int64_t queue = renderingMaterial->renderQueue;
            queue -= SHRT_MIN;

            int64_t shaderID = (int64_t)(renderingMaterial->GetShader()->GetUID() % USHRT_MAX);
            // ToDo inputs hash

            return queue * USHRT_MAX * USHRT_MAX + shaderID * USHRT_MAX;
        }

        virtual void RenderEntity(const obj_ptr<Camera>& camera) = 0;

        static bool PriorityCompRef(const NSE_VisualEntity& a, const NSE_VisualEntity& b) // ToDo
        {
            return (int64_t)*a < (int64_t)*b;
        }

    public:
        NSE_Material renderingMaterial = nullptr;
        // XMVECTOR aabb;
    };
}

#endif //VISUALENTITY_H
