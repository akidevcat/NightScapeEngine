#ifndef INPUT_SERVER_H
#define INPUT_SERVER_H

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

#include "BaseServer.h"

namespace NSE
{
    class InputServer : public BaseServer<InputServer>
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
        [[nodiscard]] bool GetKeyDown(int key) const;
        void SetMouseLocked(bool state);
        bool GetMouseLocked() const { return _isMouseLocked; }
        bool GetLMB() const;
        bool GetLMBDown() const;

    private:
        bool ReadKeyboard();
        bool ReadMouse();
        void ProcessInput();

    private:
        IDirectInput8* _directInput;
        IDirectInputDevice8* _keyboard;
        IDirectInputDevice8* _mouse;
        unsigned char _keyboardState[256] = {};
        unsigned char _pKeyboardState[256] = {};
        DIMOUSESTATE _mouseState;
        DIMOUSESTATE _pMouseState;
        bool _isMouseLocked = false;
        HWND _hwnd;
        int _screenWidth, _screenHeight, _mouseX, _mouseY, _mouseDX, _mouseDY;
    };
}

#endif //INPUT_SERVER_H
