#include "RenderServer.h"
#include <gl/glew.h>
#include "API/OpenGL/glRenderAPI.h"
#include "API/Vulkan/vkRenderAPI.h"

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
            _api = new glRenderAPI{_config, _window};
            break;
        case EngineConfiguration::RenderAPI::Vulkan:
            _api = new vkRenderAPI{_config, _window};
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