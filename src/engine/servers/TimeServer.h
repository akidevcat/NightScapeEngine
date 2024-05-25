#ifndef TIMESERVER_H
#define TIMESERVER_H

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
    long long int _absoluteTimeMs = 0;
    long long int _lastAbsoluteTimeMs = 0;
};

#endif //TIMESERVER_H
