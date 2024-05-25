#ifndef ENGINE_H
#define ENGINE_H

#include "servers/InputServer.h"
#include "servers/RenderServer.h"
#include "servers/SceneServer.h"
#include "servers/TimeServer.h"

class IGame
{
public:
    virtual ~IGame() {}
    virtual void Start() = 0;
    virtual void OnFrameInput() = 0;
    virtual void OnFrameUpdate() = 0;
    virtual void OnFrameRender() = 0;
    virtual void OnFrameCleanup() = 0;
};

class Engine
{
public:
    Engine();
    ~Engine();

    bool Initialize(IGame* game, HINSTANCE histance, int screenWidth, int screenHeight, HWND hwnd);

    void Start();
    bool UpdateFrame();

    void Shutdown();

public:
    [[nodiscard]] RenderServer* GetRenderServer() const { return _renderServer; }
    [[nodiscard]] InputServer* GetInputServer() const { return _inputServer; }
    [[nodiscard]] SceneServer* GetSceneServer() const { return _sceneServer; }
    [[nodiscard]] TimeServer* GetTimeServer() const { return _timeServer; }

private:
    void OnFrameInput();
    void OnFrameUpdate();
    void OnFrameRender();
    void OnFrameCleanup();

private:
    IGame* _gameInstance = nullptr;
    RenderServer* _renderServer = nullptr;
    InputServer* _inputServer = nullptr;
    SceneServer* _sceneServer = nullptr;
    TimeServer* _timeServer = nullptr;
};

#endif //ENGINE_H
