#include "Engine.h"

#include <iostream>

#include "../../../../pch.h"
#include "../render/RenderServer.h"
#include "../scene/SceneServer.h"

NSE::Engine::Engine(IGameInstance *game)
{
    _game = game;
}

void NSE::Engine::Initialize(const EngineConfiguration& config)
{
    if (!_game)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "Game Instance is null" << std::endl;
        return;
    }

    _config = config;

    auto sdlResult = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
    if (sdlResult < 0)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "SDL failed to initialize" << std::endl;
        return;
    }

    _window = SDL_CreateWindow(_game->GetName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, 0);
    if (!_window)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "SDL failed to create a window" << std::endl;
        return;
    }

    _surface = SDL_GetWindowSurface(_window);
    if (!_surface)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "SDL failed to create a window surface" << std::endl;
        return;
    }

    _glContext = SDL_GL_CreateContext(_window);
    if (!_glContext)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "SDL failed to create gl context" << std::endl;
        return;
    }

    auto glewResult = glewInit();
    if (glewResult != GLEW_OK)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "GLEW failed to initialize" << std::endl;
        return;
    }

    // Enable VSync
    int vSyncResult = -1;
    switch (config.vSyncMode)
    {
        case EngineConfiguration::VSyncMode::None:
            vSyncResult = SDL_GL_SetSwapInterval(0);
            break;
        case EngineConfiguration::VSyncMode::Normal:
            vSyncResult = SDL_GL_SetSwapInterval(1);
        break;
        case EngineConfiguration::VSyncMode::Adaptive:
            vSyncResult = SDL_GL_SetSwapInterval(-1);
        break;
    }
    assert(vSyncResult >= 0);

    // Initialize servers
    RegisterServer(new SceneServer{});
    RegisterServer(new RenderServer{});

    _isInitialized = true;
}

void NSE::Engine::UpdateFrame()
{
    GetServer<RenderServer>()->RenderFrame();
}

void NSE::Engine::Shutdown()
{
    for (auto server = _servers.rbegin(); server != _servers.rend(); ++server)
    {
        (*server)->OnDispose();
        free(*server);
    }

    _serversMap.clear();

    SDL_Quit();
    exit(0);
}

void NSE::Engine::RegisterServer(EngineServer* server)
{
    assert(server);

    if (!server)
        return;

    _servers.emplace_back(server);
    _serversMap.emplace(typeid(server).hash_code(), server);

    server->OnInitialize();
}
