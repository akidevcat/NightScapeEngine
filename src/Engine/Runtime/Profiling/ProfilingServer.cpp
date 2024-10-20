#include "ProfilingServer.h"

NSE::ProfilingServer::ProfilingServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::ProfilingServer::~ProfilingServer()
{
}

bool NSE::ProfilingServer::OnInitialize()
{
    return true;
}

void NSE::ProfilingServer::OnDispose()
{

}
