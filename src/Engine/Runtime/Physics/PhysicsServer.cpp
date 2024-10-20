#include "PhysicsServer.h"

NSE::PhysicsServer::PhysicsServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::PhysicsServer::~PhysicsServer()
{
}

bool NSE::PhysicsServer::OnInitialize()
{
    return true;
}

void NSE::PhysicsServer::OnDispose()
{

}
