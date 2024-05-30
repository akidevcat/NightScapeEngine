#ifndef INPUT_SERVER_H
#define INPUT_SERVER_H

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class InputServer
{
public:
    InputServer();
    ~InputServer();

    bool Initialize(HINSTANCE, HWND, int, int);
    void Shutdown();
    bool Update();

    void GetMouseLocation(int&, int&) const;
    void GetMouseDelta(int& dx, int& dy) const;
    [[nodiscard]] bool GetKey(int key) const;
    void SetMouseLocked(bool state) const;
    bool GetLMB() const;

private:
    bool ReadKeyboard();
    bool ReadMouse();
    void ProcessInput();

private:
    IDirectInput8* _directInput;
    IDirectInputDevice8* _keyboard;
    IDirectInputDevice8* _mouse;
    unsigned char _keyboardState[256];
    DIMOUSESTATE _mouseState;
    HWND _hwnd;
    int _screenWidth, _screenHeight, _mouseX, _mouseY, _mouseDX, _mouseDY;
};

#endif //INPUT_SERVER_H
