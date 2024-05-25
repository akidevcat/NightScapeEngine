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
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()).count();

    _absoluteTimeMs = now;

    auto deltaTimeMs = now - _lastAbsoluteTimeMs;
    if (!_lastAbsoluteTimeMs)
    {
        deltaTimeMs = 1;
    }

    float deltaTime = static_cast<float>(deltaTimeMs) / 1000.0f;

    _deltaTimeSeconds = deltaTime;
    _timeSeconds += deltaTime;
}

void TimeServer::EndFrame()
{
    _lastAbsoluteTimeMs = _absoluteTimeMs;
}