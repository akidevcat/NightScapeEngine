#ifndef VISUALENTITY_H
#define VISUALENTITY_H

// #include "Camera.h"
#include "SceneEntity.h"
#include "../render/Material.h"
#include <cstdint>

#define NSE_VisualEntity obj_ptr<NSE::VisualEntity>

namespace NSE
{
    class Camera;

    class VisualEntity : public SceneEntity
    {
    public:
        VisualEntity() = default;
        ~VisualEntity() override = 0;

        explicit operator uint64_t() const
        {
            if (!renderingMaterial)
            {
                return 0;
            }

            uint64_t result = 0;

            auto queue = (uint64_t)renderingMaterial->renderQueue;
            result += queue << (sizeof(uint64_t) - sizeof(short));

            auto shaderID = (uint64_t)(renderingMaterial->GetShader()->GetUID() % USHRT_MAX);
            result += shaderID << (sizeof(uint64_t) - sizeof(short) * 2);

            // ToDo inputs hash

            return result;
        }

        virtual void RenderEntity(const obj_ptr<Camera>& camera) = 0;

        static bool PriorityCompRef(const NSE_VisualEntity& a, const NSE_VisualEntity& b) // ToDo
        {
            return (uint64_t)*a < (uint64_t)*b;
        }

    public:
        NSE_Material renderingMaterial = nullptr;
        // XMVECTOR aabb;
    };
}

#endif //VISUALENTITY_H
