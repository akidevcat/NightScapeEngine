#include "OpenGLRenderAPI.h"

NSE::OpenGLRenderAPI::OpenGLRenderAPI(EngineConfiguration config, SDL_Window* window)
{
    _config = config;
    _window = window;
}

bool NSE::OpenGLRenderAPI::OnInitialize()
{
    // Use OpenGL 3.2
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    // Double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Use double buffering (0 to not use it)
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);  // Buffer has a depth of 24 bits

    _glContext = SDL_GL_CreateContext(_window);
    if (!_glContext)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "SDL failed to create gl context:" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return false;
    }

    auto glewResult = glewInit();
    if (glewResult != GLEW_OK)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "GLEW failed to initialize" << std::endl;
        return false;
    }

    // Enable VSync
    int vSyncResult = -1;
    switch (_config.vSyncMode)
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

    _glProgramID = glCreateProgram();

    return true;
}

void NSE::OpenGLRenderAPI::OnDispose()
{
    glDeleteProgram(_glProgramID);
    _glProgramID = 0;
}

void NSE::OpenGLRenderAPI::Present() const
{
    SDL_GL_SwapWindow(_window);
}

void NSE::OpenGLRenderAPI::ClearRenderTargetColor(float4 color) const
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void NSE::OpenGLRenderAPI::ClearRenderTargetDepth(float depth) const
{
    glClearDepth(depth);
}

void NSE::OpenGLRenderAPI::ClearRenderTargetStencil(int stencil) const
{
    glClearStencil(stencil);
}