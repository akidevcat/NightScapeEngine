#pragma once

#include "../IRenderAPI.h"
#include "../../Core/EngineConfiguration.h"

namespace NSE
{
    class OpenGLRenderAPI : public IRenderAPI
    {
    public:
        explicit OpenGLRenderAPI(EngineConfiguration config, SDL_Window* window);

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        EngineConfiguration _config;

        GLuint _glProgramID = 0;
        SDL_GLContext _glContext = nullptr;
        SDL_Window* _window = nullptr;
    };
}
