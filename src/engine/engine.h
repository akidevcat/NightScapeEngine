#ifndef ENGINE_H
#define ENGINE_H

#include "servers/ApplicationServer.h"
#include "servers/AssetServer.h"
#include "servers/InputServer.h"
#include "servers/RenderPipelineServer.h"
#include "servers/RenderServer.h"
#include "servers/SceneServer.h"
#include "servers/ScreenServer.h"
#include "servers/TimeServer.h"

namespace NSE
{
    class IGame
    {
    public:
        virtual ~IGame() {}
        virtual void Start() = 0;
        virtual void OnFrameInput() = 0;
        virtual void OnBeginFrameUpdate() = 0;
        virtual void OnFinishFrameUpdate() = 0;
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

        [[nodiscard]] ObjectServer* GetObjectServer() const { return _objectServer; }
        [[nodiscard]] ApplicationServer* GetApplicationServer() const { return _applicationServer; }
        [[nodiscard]] RenderServer* GetRenderServer() const { return _renderServer; }
        [[nodiscard]] InputServer* GetInputServer() const { return _inputServer; }
        [[nodiscard]] SceneServer* GetSceneServer() const { return _sceneServer; }
        [[nodiscard]] TimeServer* GetTimeServer() const { return _timeServer; }
        [[nodiscard]] int GetScreenWidth() const { return _screenWidth; }
        [[nodiscard]] int GetScreenHeight() const { return _screenHeight; }
        [[nodiscard]] float GetScreenAspect() const { return (float)_screenWidth / (float)_screenHeight; }

    private:
        void OnFrameInput();
        void OnFrameUpdate();
        void OnFrameRender();
        void OnFrameCleanup();

    private:
        IGame* _gameInstance = nullptr;
        ObjectServer* _objectServer = nullptr;
        ApplicationServer* _applicationServer = nullptr;
        RenderServer* _renderServer = nullptr;
        RenderPipelineServer* _renderPipelineServer = nullptr;
        InputServer* _inputServer = nullptr;
        SceneServer* _sceneServer = nullptr;
        TimeServer* _timeServer = nullptr;
        AssetsServer* _assetsServer = nullptr;
        ScreenServer* _screenServer = nullptr;
        int _screenWidth;
        int _screenHeight;
    };
}

#endif //ENGINE_H
