#ifndef FREECAMERA_H
#define FREECAMERA_H

#include "Camera.h"
#include "../servers/InputServer.h"
#include "../servers/TimeServer.h"

class FreeCamera : public Camera
{
public:
    FreeCamera(InputServer* input, TimeServer* time);
    void OnUpdate() override;

private:
    InputServer* _input;
    TimeServer* _time;
};

#endif //FREECAMERA_H
