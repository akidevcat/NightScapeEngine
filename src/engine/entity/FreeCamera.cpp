#include "FreeCamera.h"

#include <iostream>

#include "../servers/TimeServer.h"

using namespace DirectX;

FreeCamera::FreeCamera(InputServer* input, TimeServer* time) :
    _input(input), _time(time)
{

}

void FreeCamera::OnUpdate()
{
    auto modelMat = GetModelMatrix({});

    auto right = XMVector3Normalize(modelMat.r[0]);
    auto up = XMVector3Normalize(modelMat.r[1]);
    // auto up = XMVector3Normalize({modelMat.r[0].m128_f32., modelMat.r[1], modelMat.r[2]});
    auto forward = XMVector3Normalize(modelMat.r[2]);

    forward = XMVector3Rotate({0, 0, 1}, rotation);

    // std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
    // std::cout << "right: " << right.m128_f32[0] << " " << right.m128_f32[1] << " " << right.m128_f32[2] << std::endl;
    // std::cout << "up: " << up.m128_f32[0] << " " << up.m128_f32[1] << " " << up.m128_f32[2] << std::endl;
    // std::cout << "forward: " << forward.m128_f32[0] << " " << forward.m128_f32[1] << " " << forward.m128_f32[2] << std::endl;

    // std::cout <<

    float speed = 3.0f;

    if (_input->GetKey(DIK_W))
    {
        position += forward * _time->Delta() * speed;
    }
    if (_input->GetKey(DIK_S))
    {
        position += -forward * _time->Delta() * speed;
    }
    if (_input->GetKey(DIK_D))
    {
        position += right * _time->Delta() * speed;
    }
    if (_input->GetKey(DIK_A))
    {
        position += -right * _time->Delta() * speed;
    }
    if (_input->GetKey(DIK_SPACE))
    {
        position += up * _time->Delta() * speed;
    }
    if (_input->GetKey(DIK_LCONTROL))
    {
        position += -up * _time->Delta() * speed;
    }

    int dx, dy;
    _input->GetMouseDelta(dx, dy);

    auto camRot = XMQuaternionRotationRollPitchYaw((float)dy / 300.0f, (float)dx / 300.0f, 0);
    rotation = XMQuaternionMultiply(camRot, rotation);
}
