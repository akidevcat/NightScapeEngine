#include "application.h"

#include "engine/servers/InputServer.h"
#include "game/game.h"

Application::Application()
{
    // _Input = 0;
    // _Application = 0;
}

Application::~Application()
{

}

bool Application::Initialize()
{
    bool result = true;
    int screenWidth = 0, screenHeight = 0;

    InitializeWindows(screenWidth, screenHeight);

	_engine = new Engine{};
	_game = new Game{};

	result = _engine->Initialize(_game, _hinstance, screenWidth, screenHeight, _hwnd);
	if (!result)
	{
	    return false;
	}

    result = _game->Initialize(_engine);
    if (!result)
    {
        return false;
    }

    return true;
}

void Application::Shutdown()
{
    // Release the application class object.
    if (_game) // ToDo transfer to engine
    {
        _game->Shutdown();
        delete _game;
        _game = nullptr;
    }

	if (_engine)
	{
		_engine->Shutdown();
		delete _engine;
		_engine = nullptr;
    }

    ShutdownWindows();
}

void Application::Run()
{
    MSG msg;
    bool done = false;

    // Initialize the message structure.
    ZeroMemory(&msg, sizeof(MSG));

	_engine->Start();

    // Loop until there is a quit message from the window or the user.
    while (!done)
    {
        // Handle the windows messages.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // If windows signals to end the application then exit out.
        if (msg.message == WM_QUIT)
        {
            done = true;
        }
        else
        {
            // Otherwise do the frame processing.
            if (!Frame())
            {
                done = true;
            }
        }
    }
}

bool Application::Frame()
{
    bool result = true;

	if (!_engine->UpdateFrame())
	{
		return false;
	}

    return true;
}

LRESULT CALLBACK Application::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void Application::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	_hinstance = GetModuleHandle(nullptr);

	// Give the application a name.
	_applicationName = "Night Scape Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = _hinstance;
	wc.hIcon		 = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = _applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	// if(_engine->GetRenderServer()->GetFullscreenState())
	// ToDo
	if(false)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = static_cast<unsigned long>(screenWidth);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(screenHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	// _hwnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName,
	// 					    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
	// 					    posX, posY, screenWidth, screenHeight, nullptr, nullptr, _hinstance, nullptr);
	_hwnd = CreateWindowEx(0, _applicationName, _applicationName,
							WS_OVERLAPPED,
							posX, posY, screenWidth, screenHeight, nullptr, nullptr, _hinstance, nullptr);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	// Hide the mouse cursor.
	// ShowCursor(false);
}

void Application::ShutdownWindows()
{
	// Show the mouse cursor.
	// ShowCursor(true);

	// ToDo
	// Fix the display settings if leaving full screen mode.
	// if(_engine->GetRenderServer()->GetFullscreenState())
	// {
	// 	ChangeDisplaySettings(nullptr, 0);
	// }

	// Remove the window.
	DestroyWindow(_hwnd);
	_hwnd = nullptr;

	// Remove the application instance.
	UnregisterClass(_applicationName, _hinstance);
	_hinstance = nullptr;

	// Release the pointer to this class.
	ApplicationHandle = nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}