#include "ApplicationServer.h"

#include "../../../Vendor/ImGui/imgui_impl_sdl2.h"

NSE::ApplicationServer::ApplicationServer(EngineConfiguration cfg)
{
    _config = cfg;
}

NSE::ApplicationServer::~ApplicationServer()
{
}

bool NSE::ApplicationServer::OnInitialize()
{
    // ImGui_ImplSDL2_Init();

    return true;
}

void NSE::ApplicationServer::OnDispose()
{

}

bool NSE::ApplicationServer::Update()
{
    SDL_Event windowEvent;
    while(SDL_PollEvent(&windowEvent))
    {
        if(windowEvent.type == SDL_QUIT)
        {
            return false;
        }

        // ImGui_ImplSDL2_ProcessEvent(&windowEvent);
    }
    return true;
}
