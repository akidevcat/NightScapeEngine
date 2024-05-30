#include "Camera.h"

Camera::Camera()
{
    UpdateProjectionMatrix();
}

Camera::~Camera()
{

}

void Camera::SetParams(float aspect, float fov, float nearPane, float farPlane, bool isOrthographic, float orthographicSize)
{
    _aspect = aspect;
    _fov = fov;
    _near = nearPane;
    _far = farPlane;
    _isOrthographic = isOrthographic;

    if (isOrthographic)
    {
        _orthographicSize = orthographicSize;
    }

    UpdateProjectionMatrix();
}

void Camera::GetParams(float *aspect, float *fov, float *nearPane, float *farPlane, float *orthographicSize) const
{
    if (aspect)
    {
        *aspect = _aspect;
    }

    if (fov)
    {
        *fov = _fov;
    }

    if (nearPane)
    {
        *nearPane = _near;
    }

    if (farPlane)
    {
        *farPlane = _far;
    }

    if (orthographicSize)
    {
        *orthographicSize = _orthographicSize;
    }
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{

    return DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, DirectX::XMQuaternionInverse(rotation), DirectX::g_XMZero);
}

void Camera::UpdateProjectionMatrix()
{
    if (_isOrthographic)
    {
        _projectionMatrix = DirectX::XMMatrixOrthographicLH(_orthographicSize, _orthographicSize * _aspect, _near, _far); // ToDo review
    }
    else
    {
        _projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(_fov), _aspect, _near, _far);
    }
}

