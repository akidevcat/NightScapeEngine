#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"
#include "IRenderAPI.h"
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
        IRenderAPI* _api = nullptr;
        EngineConfiguration _config;
        SDL_Window* _window;
    };

    constexpr EngineServerAccessor<RenderServer> Render;
}