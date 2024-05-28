#ifndef TIMESERVER_H
#define TIMESERVER_H

#include <chrono>

class TimeServer
{
public:
    TimeServer();
    ~TimeServer();
    bool Initialize();

    void BeginFrame();
    void EndFrame();

public:
    [[nodiscard]] float Time() const { return _timeSeconds; }
    [[nodiscard]] float Delta() const { return _deltaTimeSeconds; }

private:
    float _timeSeconds = 0;
    float _deltaTimeSeconds = 0;
    std::chrono::time_point<std::chrono::system_clock> _absoluteTimeMs = {};
    std::chrono::time_point<std::chrono::system_clock> _lastAbsoluteTimeMs = {};
};

#endif //TIMESERVER_H
