#include "application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    Application* app;

    app = new Application;

    if(app->Initialize())
    {
        app->Run();
    }

    app->Shutdown();
    delete app;
    app = nullptr;

    return 0;
}