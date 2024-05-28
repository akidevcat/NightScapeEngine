#include "TimeServer.h"

#include <chrono>

TimeServer::TimeServer()
{

}

TimeServer::~TimeServer()
{

}

bool TimeServer::Initialize()
{
    return true;
}

void TimeServer::BeginFrame()
{
    // auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
    // std::chrono::system_clock::now().time_since_epoch());
    auto now = std::chrono::system_clock::now();

    _absoluteTimeMs = now;

    auto deltaTimeMs = now - _lastAbsoluteTimeMs;
    if (_lastAbsoluteTimeMs.time_since_epoch().count() < 1)
    {
        deltaTimeMs = std::chrono::milliseconds{1};
    }

    float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(deltaTimeMs).count() / 1000000.0f;

    _deltaTimeSeconds = deltaTime;
    _timeSeconds += deltaTime;
}

void TimeServer::EndFrame()
{
    _lastAbsoluteTimeMs = _absoluteTimeMs;
}