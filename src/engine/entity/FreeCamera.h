#ifndef FREECAMERA_H
#define FREECAMERA_H

#include "Camera.h"
#include "../servers/InputServer.h"
#include "../servers/TimeServer.h"

#define NSE_FreeCamera obj_ptr<NSE::FreeCamera>

namespace NSE
{
    class FreeCamera : public Camera
    {
    public:
        FreeCamera() = default;
        ~FreeCamera() override = default;

        void OnUpdate() override;
    };
}

#endif //FREECAMERA_H
