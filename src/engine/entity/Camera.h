#ifndef CAMERA_H
#define CAMERA_H

// #include "SceneEntity.h"
#include "../render/RenderTexture.h"
#include "../scene/Scene.h"

#define NSE_Camera obj_ptr<NSE::Camera>

namespace NSE
{
    class SceneEntity;

    enum CameraClearMode
    {
        CAMERA_CLEAR_MODE_COLOR,
        CAMERA_CLEAR_MODE_DEPTH,
        CAMERA_CLEAR_MODE_NOTHING
    };

    class Camera : public SceneEntity
    {
    public:
        Camera();
        ~Camera() override;

        void SetParams(float aspect, float fov, float nearPlane, float farPlane, bool isOrthographic, float orthographicSize);
        void GetParams(float* aspect, float* fov, float* nearPlane, float* farPlane, float* orthographicSize) const;
        void CopyParams(const NSE_Camera& other);

        void SetFov(float fov);

        [[nodiscard]] bool IsOrthographic() const { return _isOrthographic; }
        [[nodiscard]] DirectX::XMMATRIX GetProjectionMatrix() const { return _projectionMatrix; }
        [[nodiscard]] DirectX::XMMATRIX GetViewMatrix() const;

        static bool PriorityComp(Camera* a, Camera* b) // ToDo
        {
            return a->priority < b->priority;
        }

        static bool PriorityCompRef(const NSE_Camera& a, const NSE_Camera& b) // ToDo
        {
            return a->priority < b->priority;
        }

        void UpdateProjectionMatrix();

        int                 priority = 0;
        NSE_RenderTexture   colorTarget = nullptr;
        NSE_RenderTexture   depthTarget = nullptr;
        Scene*              targetScene = nullptr;
        CameraClearMode     clearMode = CAMERA_CLEAR_MODE_COLOR;
        DirectX::XMFLOAT4   clearColor = {0, 0, 0, 1};

    private:
        float _fov = 60.0f;
        float _near = 0.1f;
        float _far = 1000.0f;
        float _aspect = 1.0f;

        bool _isOrthographic = false;
        float _orthographicSize = 10.0;

        DirectX::XMMATRIX _projectionMatrix;
    };
}

#endif //CAMERA_H
