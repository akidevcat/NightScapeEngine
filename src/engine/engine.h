#ifndef ENGINE_H
#define ENGINE_H

#include "servers/InputServer.h"
#include "servers/RenderServer.h"
#include "servers/SceneServer.h"

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

    RenderServer* GetRenderServer() const { return _renderServer; }
    InputServer* GetInputServer() const { return _inputServer; }
    SceneServer* GetSceneServer() const { return _sceneServer; }

    void Shutdown();

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
};

#endif //ENGINE_H
