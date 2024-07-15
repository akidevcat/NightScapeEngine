#pragma once

#include <directxmath.h>

#include "Entity.h"
#include "../math/Math.h"

#define NSE_SceneEntity obj_ptr<NSE::SceneEntity>

namespace NSE
{
    class Scene;

    class SceneEntity : public Entity
    {
    public:
        SceneEntity() = default;
        // ~SceneEntity() override = 0;
        ~SceneEntity() override = 0;

        [[nodiscard]] DirectX::XMMATRIX GetModelMatrix(Vector3d relativeTo, float scaling = 1.0f) const;
        [[nodiscard]] DirectX::XMMATRIX GetModelMatrixUI() const;
        [[nodiscard]] DirectX::XMVECTOR Forward() const;
        [[nodiscard]] DirectX::XMVECTOR Right() const;
        [[nodiscard]] DirectX::XMVECTOR Up() const;

        [[nodiscard]] Scene* GetScene() const;
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