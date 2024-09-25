#include "Engine.h"

#include <iostream>

// #include "../../../../pch.h"
#include "../Render/RenderPipelineServer.h"
#include "../Render/RenderServer.h"
#include "../Scene/SceneServer.h"

NSE::Engine::Engine(IAppInstance *app)
{
    _app = app;
}

void NSE::Engine::Initialize(const EngineConfiguration& config)
{
    if (!_app)
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
        std::cerr << "SDL failed to initialize:" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return;
    }

    _window = SDL_CreateWindow(_app->GetName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, SDL_WINDOW_OPENGL);
    if (!_window)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "SDL failed to create a window:" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return;
    }

    _surface = SDL_GetWindowSurface(_window);
    if (!_surface)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "SDL failed to create a window surface:" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return;
    }

    // Initialize servers
    RegisterServer(new SceneServer{});
    RegisterServer(new RenderServer{config, _window});
    RegisterServer(new RenderPipelineServer{});

    _isInitialized = true;
}

void NSE::Engine::UpdateFrame()
{
    assert(_isInitialized);

    GetServer<RenderPipelineServer>()->RenderFrame();
}

void NSE::Engine::Shutdown()
{
    for (auto server = _servers.rbegin(); server != _servers.rend(); ++server)
    {
        (*server)->OnDispose();
        free(*server);
    }

    _serversMap.clear();

    SDL_DestroyWindow(_window);
    _window = nullptr;

    SDL_Quit();
    exit(0);
}

// void NSE::Engine::RegisterServer(EngineServer* server)
// {
//     assert(server);
//
//     if (!server)
//         return;
//
//     _servers.emplace_back(server);
//     _serversMap.emplace(typeid(*server).hash_code(), server);
//
//     server->OnInitialize();
// }
