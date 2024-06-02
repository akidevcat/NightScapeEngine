#include "InputServer.h"

#include <iostream>

NSE::InputServer::InputServer()
{
    _directInput = nullptr;
    _keyboard = nullptr;
    _mouse = nullptr;
}

NSE::InputServer::~InputServer()
{

}

bool NSE::InputServer::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
    HRESULT result;
    _hwnd = hwnd;

    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    _mouseX = 0;
    _mouseY = 0;

    result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&_directInput), nullptr);
    if(FAILED(result))
    {
        return false;
    }

    // Initialize the direct input interface for the keyboard.
    result = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, nullptr);
    if(FAILED(result))
    {
        return false;
    }

    // Set the data format.  In this case since it is a keyboard we can use the predefined data format.
    result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
    if(FAILED(result))
    {
        return false;
    }

    // Set the cooperative level of the keyboard to not share with other programs.
    // result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    result = _keyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if(FAILED(result))
    {
        return false;
    }

    // Now acquire the keyboard.
    result = _keyboard->Acquire();
    if(FAILED(result))
    {
        return false;
    }

    // Initialize the direct input interface for the mouse.
    result = _directInput->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
    if(FAILED(result))
    {
        return false;
    }

    // Set the data format for the mouse using the pre-defined mouse data format.
    result = _mouse->SetDataFormat(&c_dfDIMouse);
    if(FAILED(result))
    {
        return false;
    }

    // Set the cooperative level of the mouse to share with other programs.
    result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if(FAILED(result))
    {
        return false;
    }

    // Acquire the mouse.
    result = _mouse->Acquire();
    if(FAILED(result))
    {
        return false;
    }

    return true;
}

void NSE::InputServer::Shutdown()
{
    // Release the mouse.
    if(_mouse)
    {
        _mouse->Unacquire();
        _mouse->Release();
        _mouse = nullptr;
    }

    // Release the keyboard.
    if(_keyboard)
    {
        _keyboard->Unacquire();
        _keyboard->Release();
        _keyboard = nullptr;
    }

    // Release the main interface to direct input.
    if(_directInput)
    {
        _directInput->Release();
        _directInput = nullptr;
    }
}

bool NSE::InputServer::Update()
{
    bool result;

    // Read the current state of the keyboard.
    result = ReadKeyboard();
    if(!result)
    {
        return false;
    }

    // Read the current state of the mouse.
    result = ReadMouse();
    if(!result)
    {
        return false;
    }

    // Process the changes in the mouse and keyboard.
    ProcessInput();

    return true;
}

bool NSE::InputServer::ReadKeyboard()
{
    HRESULT result;

    std::swap(_keyboardState, _pKeyboardState);

    // Read the keyboard device.
    result = _keyboard->GetDeviceState(sizeof(_keyboardState), &_keyboardState);
    if(FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get control back.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            _keyboard->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool NSE::InputServer::ReadMouse()
{
    HRESULT result;

    std::swap(_mouseState, _pMouseState);

    // Read the mouse device.
    result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), &_mouseState);
    if(FAILED(result))
    {
        // If the mouse lost focus or was not acquired then try to get control back.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            _mouse->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}

void NSE::InputServer::ProcessInput()
{
    // Update the location of the mouse cursor based on the change of the mouse location during the frame.
    _mouseX += _mouseState.lX;
    _mouseY += _mouseState.lY;
    _mouseDX = _mouseState.lX;
    _mouseDY = _mouseState.lY;

    // Ensure the mouse location doesn't exceed the screen width or height.
    if(_mouseX < 0)  { _mouseX = 0; }
    if(_mouseY < 0)  { _mouseY = 0; }

    if(_mouseX > _screenWidth)  { _mouseX = _screenWidth; }
    if(_mouseY > _screenHeight) { _mouseY = _screenHeight; }
}

// bool InputServer::IsEscapePressed()
// {
//     // Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
//     if(_keyboardState[DIK_ESCAPE] & 0x80)
//     {
//         return true;
//     }
//
//     return false;
// }

void NSE::InputServer::GetMouseLocation(int& mouseX, int& mouseY) const
{
    mouseX = _mouseX;
    mouseY = _mouseY;
}

void NSE::InputServer::GetMouseDelta(int &dx, int &dy) const
{
    dx = _mouseDX;
    dy = _mouseDY;
}

bool NSE::InputServer::GetKey(int key) const
{
    if(_keyboardState[key] & 0x80)
    {
        return true;
    }

    return false;
}

bool NSE::InputServer::GetKeyDown(int key) const
{
    return (_keyboardState[key] & 0x80) && !(_pKeyboardState[key] & 0x80);
}

void NSE::InputServer::SetMouseLocked(const bool state)
{
    _mouse->Unacquire();
    if (FAILED(_mouse->SetCooperativeLevel(_hwnd, state ? (DISCL_FOREGROUND | DISCL_EXCLUSIVE) : (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
    {

        std::cerr << "Failed to set mouse locked state" << std::endl;
    }
    _mouse->Acquire();
    _isMouseLocked = state;
}

bool NSE::InputServer::GetLMB() const
{
    return _mouseState.rgbButtons[0] & 0x80;
}

bool NSE::InputServer::GetLMBDown() const
{
    return (_mouseState.rgbButtons[0] & 0x80) && !(_pMouseState.rgbButtons[0] & 0x80);
}

