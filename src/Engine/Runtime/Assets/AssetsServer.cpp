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
    return true;
}

void NSE::AssetsServer::OnDispose()
{

}
