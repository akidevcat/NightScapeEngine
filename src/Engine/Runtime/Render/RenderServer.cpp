#include "RenderServer.h"
#include <gl/glew.h>
#include "API/OpenGLRenderAPI.h"

NSE::RenderServer::RenderServer(EngineConfiguration cfg, SDL_Window* window)
{
    _config = cfg;
    _window = window;
}

NSE::RenderServer::~RenderServer()
{
    free(_api);
}

bool NSE::RenderServer::OnInitialize()
{
    switch (_config.renderAPI)
    {
        case EngineConfiguration::RenderAPI::OpenGL:
            _api = new OpenGLRenderAPI{_config, _window};
            break;
    }

    assert(_api);
    if (!_api)
        return false;

    if (!_api->OnInitialize())
        return false;

    return true;
}

void NSE::RenderServer::OnDispose()
{
    if (_api)
        _api->OnDispose();
}

void NSE::RenderServer::Present()
{
    SDL_GL_SwapWindow(_window);
}

void NSE::RenderServer::ClearRenderTarget()
{
    glClearColor(1, 0, 0, 1);
}
