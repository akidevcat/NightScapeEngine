#include "ApplicationServer.h"

NSE::ApplicationServer::ApplicationServer()
{
}

NSE::ApplicationServer::~ApplicationServer()
{
}

void NSE::ApplicationServer::Exit()
{
    _isExitState = true;
}

bool NSE::ApplicationServer::Update()
{
    if (_isExitState)
        return false;

    return true;
}
