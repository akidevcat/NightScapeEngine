#include "TimeServer.h"

NSE::TimeServer::TimeServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::TimeServer::~TimeServer()
{
}

bool NSE::TimeServer::OnInitialize()
{
    return true;
}

void NSE::TimeServer::OnDispose()
{

}

void NSE::TimeServer::BeginFrame()
{
}

void NSE::TimeServer::EndFrame()
{
}
