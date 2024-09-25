#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"
#include "IRenderPipeline.h"

namespace NSE
{
    class Engine;

    class RenderServer : public EngineServer<RenderServer>
    {
    public:
        RenderServer(EngineConfiguration cfg, SDL_Window* window);
        ~RenderServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    public:
        void Present();
        void ClearRenderTarget();

    private:
        static RenderServer* _instance;

    public:
        static RenderServer* Get()
        {
            assert(_instance);
            return _instance;
        }

    private:
        EngineConfiguration _cfg;
        GLuint _glProgramID;

        SDL_GLContext _glContext = nullptr;
        SDL_Window* _window = nullptr;
    };

    constexpr EngineServerAccessor<RenderServer> Render;
}