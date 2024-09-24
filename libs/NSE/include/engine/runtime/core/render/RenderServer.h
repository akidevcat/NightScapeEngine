#pragma once

#include "../../../../pch.h"

#include "../main/EngineServer.h"

namespace NSE
{
    class Engine;

    class RenderServer : public EngineServer
    {
    public:
        RenderServer();
        ~RenderServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        void RenderFrame();

    private:
        GLuint _glProgramID;

        friend NSE::Engine;
    };
}