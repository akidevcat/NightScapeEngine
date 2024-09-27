#include "Engine.h"

#include <iostream>

#include "../Assets/AssetsServer.h"
#include "../Audio/AudioServer.h"
#include "../Input/InputServer.h"
#include "../Physics/PhysicsServer.h"
#include "../Profiling/ProfilingServer.h"
#include "../Render/RenderPipelineServer.h"
#include "../Render/RenderServer.h"
#include "../Scene/SceneServer.h"
#include "../Time/TimeServer.h"

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
    RegisterServer(new TimeServer{config});
    RegisterServer(new ProfilingServer{config});
    RegisterServer(new AssetsServer{config});
    RegisterServer(new AudioServer{config});
    RegisterServer(new InputServer{config});
    RegisterServer(new PhysicsServer{config});
    RegisterServer(new SceneServer{});
    RegisterServer(new RenderServer{config, _window});
    RegisterServer(new RenderPipelineServer{config});

    _isInitialized = true;
}

void NSE::Engine::UpdateFrame()
{
    assert(("Engine is not initialized", _isInitialized));

    GetServer<TimeServer>()->BeginFrame();
    GetServer<InputServer>()->BeginFrame();
    GetServer<SceneServer>()->BeginFrameUpdate();
    GetServer<SceneServer>()->UpdateFrame();
    GetServer<RenderPipelineServer>()->RenderFrame();
    GetServer<SceneServer>()->EndFrameUpdate();
    GetServer<InputServer>()->EndFrame();
    GetServer<TimeServer>()->EndFrame();
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
}
