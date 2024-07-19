#include "TimeServer.h"

#include <chrono>

NSE::TimeServer::TimeServer()
{

}

NSE::TimeServer::~TimeServer()
{

}

bool NSE::TimeServer::Initialize()
{
    return true;
}

void NSE::TimeServer::BeginFrame()
{
    auto now = std::chrono::system_clock::now();

    _absoluteTimeMs = now;

    auto deltaTimeMs = now - _lastAbsoluteTimeMs;
    if (_lastAbsoluteTimeMs.time_since_epoch().count() < 1)
    {
        deltaTimeMs = std::chrono::milliseconds{1};
    }

    float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(deltaTimeMs).count() / 1000000.0f;
    deltaTime *= _timeScale;

    _deltaTimeSeconds = deltaTime;
    _timeSeconds += deltaTime;
}

void NSE::TimeServer::EndFrame()
{
    _lastAbsoluteTimeMs = _absoluteTimeMs;
}

void NSE::TimeServer::SetTimeScale(float timeScale)
{
    _timeScale = timeScale;
}
