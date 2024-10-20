#ifndef NIGHT_SCAPE_ENGINE_h
#define NIGHT_SCAPE_ENGINE_h

#include "pch.h"
#include "Engine/Runtime/Core/Engine.h"
#include "Engine/Runtime/Scene/Scene.h"

namespace NSE
{
    Engine* CreateEngine(IAppInstance*&& app, IEditorInstance*&& editor = nullptr);
}

#endif //NIGHT_SCAPE_ENGINE_h