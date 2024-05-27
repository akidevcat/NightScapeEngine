#ifndef CAMERA_H
#define CAMERA_H

#include "SceneEntity.h"
#include "../scene/Scene.h"

class Camera : public SceneEntity
{
public:
    Camera();
    ~Camera();

    void SetParams(float aspect, float fov, float near, float far, bool isOrthographic, float orthographicSize);
    void GetParams(float* aspect, float* fov, float* near, float* far, float* orthographicSize) const;

    [[nodiscard]] bool IsOrthographic() const { return _isOrthographic; }
    [[nodiscard]] DirectX::XMMATRIX GetProjectionMatrix() const { return _projectionMatrix; }

    void UpdateProjectionMatrix();

private:
    float _fov = 60.0f;
    float _near = 0.1f;
    float _far = 1000.0f;
    float _aspect = 1.0f;

    bool _isOrthographic = false;
    float _orthographicSize = 10.0;

    DirectX::XMMATRIX _projectionMatrix;

    Scene* _targetScene = nullptr;
};

#endif //CAMERA_H
