#include "FreeCamera.h"

#include <iostream>

#include "../servers/TimeServer.h"

using namespace DirectX;

// NSE::FreeCamera::FreeCamera(InputServer* input, TimeServer* time) :
//     _input(input), time(time)
// {
//
// }

void NSE::FreeCamera::OnUpdate()
{
    auto input = InputServer::Get();
    auto time = TimeServer::Get();

    auto modelMat = GetModelMatrix({});

    auto right = XMVector3Normalize(modelMat.r[0]);
    auto up = XMVector3Normalize(modelMat.r[1]);
    auto forward = XMVector3Normalize(modelMat.r[2]);

    forward = XMVector3Rotate({0, 0, 1}, rotation);

    float speed = 3.0f;

    if (input->GetKey(DIK_W))
    {
        position += forward * time->Delta() * speed;
    }
    if (input->GetKey(DIK_S))
    {
        position += -forward * time->Delta() * speed;
    }
    if (input->GetKey(DIK_D))
    {
        position += right * time->Delta() * speed;
    }
    if (input->GetKey(DIK_A))
    {
        position += -right * time->Delta() * speed;
    }
    if (input->GetKey(DIK_SPACE))
    {
        position += up * time->Delta() * speed;
    }
    if (input->GetKey(DIK_LCONTROL))
    {
        position += -up * time->Delta() * speed;
    }

    int dx, dy;
    input->GetMouseDelta(dx, dy);

    auto camRot = XMQuaternionRotationRollPitchYaw((float)dy / 300.0f, (float)dx / 300.0f, 0);
    rotation = XMQuaternionMultiply(camRot, rotation);
}
