#include "AssetsServer.h"

NSE::AssetsServer::AssetsServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::AssetsServer::~AssetsServer()
{
}

bool NSE::AssetsServer::OnInitialize()
{
    _applicationBasePathPtr = SDL_GetBasePath();
    _applicationBasePath = std::string{_applicationBasePathPtr};

    return true;
}

void NSE::AssetsServer::OnDispose()
{
    SDL_free(_applicationBasePathPtr);
}
