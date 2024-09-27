#include "InputServer.h"

NSE::InputServer::InputServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::InputServer::~InputServer()
{
}

bool NSE::InputServer::OnInitialize()
{
    return true;
}

void NSE::InputServer::OnDispose()
{

}

void NSE::InputServer::BeginFrame()
{
}

void NSE::InputServer::EndFrame()
{
}
