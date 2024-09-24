#include "../../../../pch.h"

#ifdef NSE_USE_OPENGL_BACKEND

#include "RenderServer.h"

NSE::RenderServer::RenderServer()
{
    _glProgramID = 0;
}

NSE::RenderServer::~RenderServer()
{

}

bool NSE::RenderServer::OnInitialize()
{
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    _glProgramID = glCreateProgram();

    return true;
}

void NSE::RenderServer::OnDispose()
{

}

void NSE::RenderServer::RenderFrame()
{

}

#endif