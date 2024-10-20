#include "Engine.h"

#include <iostream>
#include <SDL_vulkan.h>

#include "../Application/ApplicationServer.h"
#include "../Assets/AssetsServer.h"
#include "../Audio/AudioServer.h"
#include "../Input/InputServer.h"
#include "../Physics/PhysicsServer.h"
#include "../Profiling/ProfilingServer.h"
#include "../Render/RenderPipelineServer.h"
#include "../Render/RenderServer.h"
#include "../Scene/SceneServer.h"
#include "../Time/TimeServer.h"
#include "../Memory/FactoryRegistry.h"
#include "../Serializer/SerializerServer.h"

NSE::Engine::Engine(IAppInstance* app, IEditorInstance* editor)
{
    _app = app;
    _editor = editor;
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

    Uint32 windowsFlags = 0;
    switch (config.renderAPI)
    {
        case EngineConfiguration::RenderAPI::OpenGL:
            windowsFlags |= SDL_WINDOW_OPENGL;
            break;
        case EngineConfiguration::RenderAPI::Vulkan:
            SDL_Vulkan_LoadLibrary(nullptr);
            windowsFlags |= SDL_WINDOW_VULKAN;
            break;
    }

    _window = SDL_CreateWindow(_app->GetName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, windowsFlags);
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
    RegisterServer(new SerializerServer{config});
    RegisterServer(new ApplicationServer{config});
    RegisterServer(new TimeServer{config});
    RegisterServer(new ProfilingServer{config});
    RegisterServer(new AssetsServer{config});
    RegisterServer(new AudioServer{config});
    RegisterServer(new InputServer{config});
    RegisterServer(new PhysicsServer{config});
    RegisterServer(new SceneServer{});
    RegisterServer(new RenderServer{config, _window});
    RegisterServer(new RenderPipelineServer{config});

    if (_editor)
    {
        _editor->OnInitialize(_app);
    }

    _app->OnAppSetup();
    _app->OnRegisteringComponents(GetServer<SceneServer>()->GetFactoryRegistry());
    _app->OnRegisteringEntitySystems(GetServer<SceneServer>()->GetFactoryRegistry());
    _app->OnRegisteringMainSystem();

    if (_editor)
    {
        _editor->OnInitializeAfterApp(_app);
    }

    _isInitialized = true;
}

bool NSE::Engine::UpdateFrame()
{
    assert(("Engine is not initialized", _isInitialized));

    GetServer<TimeServer>()->BeginFrame();
    if (!GetServer<ApplicationServer>()->Update())
        return false;
    GetServer<InputServer>()->BeginFrame();
    GetServer<SceneServer>()->BeginFrameUpdate();
    GetServer<SceneServer>()->UpdateFrame();
    GetServer<RenderServer>()->Update();
    GetServer<RenderPipelineServer>()->RenderFrame();
    GetServer<SceneServer>()->EndFrameUpdate();
    GetServer<InputServer>()->EndFrame();
    GetServer<TimeServer>()->EndFrame();

    return true;
}

void NSE::Engine::Shutdown()
{
    _app->OnDispose();

    if (_editor)
    {
        _editor->OnDispose();
    }

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
