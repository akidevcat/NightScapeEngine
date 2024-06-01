#ifndef APPLICATION_H
#define APPLICATION_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "engine/engine.h"
#include "game/game.h"

class Application
{
public:
    Application();
    ~Application();

    bool Initialize();
    void Shutdown();
    void Run();

    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
    bool Frame();
    void InitializeWindows(int&, int&);
    void ShutdownWindows();

private:
    LPCSTR _applicationName;
    HINSTANCE _hinstance;
    HWND _hwnd;

    NSE::Engine* _engine = nullptr;
    Game* _game = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Application* ApplicationHandle = nullptr;

#endif //APPLICATION_H
