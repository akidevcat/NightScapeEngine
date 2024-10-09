#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"
#include "IRenderAPI.h"
#include "RenderPipeline.h"

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

        void Update();

    public:
        [[nodiscard]] const IRenderAPI* api() const { return _api; }

    private:
        IRenderAPI* _api = nullptr;
        EngineConfiguration _config;
        SDL_Window* _window;
    };

    constexpr EngineServerAccessor<RenderServer> sRender;
}