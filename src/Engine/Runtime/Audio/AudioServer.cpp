#include "AudioServer.h"

NSE::AudioServer::AudioServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::AudioServer::~AudioServer()
{
}

bool NSE::AudioServer::OnInitialize()
{
    return true;
}

void NSE::AudioServer::OnDispose()
{

}
