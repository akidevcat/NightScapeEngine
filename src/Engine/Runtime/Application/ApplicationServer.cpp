#include "ApplicationServer.h"

NSE::ApplicationServer::ApplicationServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::ApplicationServer::~ApplicationServer()
{
}

bool NSE::ApplicationServer::OnInitialize()
{
    return true;
}

void NSE::ApplicationServer::OnDispose()
{

}

bool NSE::ApplicationServer::Update()
{
    SDL_Event windowEvent;
    while(SDL_PollEvent(&windowEvent))
        if(windowEvent.type == SDL_QUIT)
        {
            return false;
        }
    return true;
}
