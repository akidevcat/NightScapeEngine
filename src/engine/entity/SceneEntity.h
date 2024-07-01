#ifndef SCENEENTITY_H
#define SCENEENTITY_H

#include <directxmath.h>

#include "Entity.h"
#include "../math/Math.h"

#define NSE_SceneEntity obj_ptr<NSE::SceneEntity>

namespace NSE
{
    class SceneEntity : public Entity
    {
    public:
        SceneEntity() = default;
        ~SceneEntity() override = 0;

        [[nodiscard]] DirectX::XMMATRIX GetModelMatrix(Vector3d relativeTo) const;
        [[nodiscard]] DirectX::XMVECTOR Forward() const;
        [[nodiscard]] DirectX::XMVECTOR Right() const;
        [[nodiscard]] DirectX::XMVECTOR Up() const;

        bool GetSceneUID(size_t& uid) const;
        void SetSceneUID(size_t uid);
        void ResetSceneUID() { _sceneUID = 0; }

        virtual void OnUpdate() {}

        [[nodiscard]] bool IsEnabled() const { return _enabled; }
        void SetEnabled(bool state);

    public:
        Vector3d          position = Vector3d{};
        DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
        DirectX::XMFLOAT3 scale    = DirectX::XMFLOAT3(1, 1, 1);

    private:
        bool              _enabled = true;
        size_t            _sceneUID = 0;
    };
}

#endif //SCENEENTITY_H
