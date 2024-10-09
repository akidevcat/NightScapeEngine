#include "SerializerServer.h"

#include "../Assets/TextAsset.h"

NSE::SerializerServer::SerializerServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::SerializerServer::~SerializerServer()
{
}

bool NSE::SerializerServer::OnInitialize()
{
    RegisterSerializer<TextAssetSerializer>();

    return true;
}

void NSE::SerializerServer::OnDispose()
{

}
