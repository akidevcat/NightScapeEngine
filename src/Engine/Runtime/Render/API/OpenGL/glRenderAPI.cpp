#include "glRenderAPI.h"

#include "glGraphicsBuffer.h"

NSE::glRenderAPI::glRenderAPI(EngineConfiguration config, SDL_Window* window)
{
    _config = config;
    _window = window;
}

bool NSE::glRenderAPI::OnInitialize()
{
    // Use OpenGL 3.2
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2);
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

void NSE::glRenderAPI::OnDispose()
{
    glDeleteProgram(_glProgramID);
    _glProgramID = 0;
}

void NSE::glRenderAPI::Present() const
{
    SDL_GL_SwapWindow(_window);
}

void NSE::glRenderAPI::ClearRenderTargetColor(float4 color) const
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void NSE::glRenderAPI::ClearRenderTargetDepth(float depth) const
{
    glClearDepth(depth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void NSE::glRenderAPI::ClearRenderTargetStencil(int stencil) const
{
    glClearStencil(stencil);
    glClear(GL_STENCIL_BUFFER_BIT);
}

NSE::SRef<NSE::GraphicsBuffer> NSE::glRenderAPI::CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t size,
    bool keepDataOnCPU) const
{
    return SRef{new glGraphicsBuffer{target, size, keepDataOnCPU}};
}

NSE::SRef<NSE::GraphicsBuffer> NSE::glRenderAPI::CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t stride,
    size_t count, bool keepDataOnCPU) const
{
    return SRef{new glGraphicsBuffer{target, stride, count, keepDataOnCPU}};
}
