#ifndef NIGHT_SCAPE_ENGINE_h
#define NIGHT_SCAPE_ENGINE_h

#include "engine/runtime/core/main/Engine.h"

namespace NSE
{
    Engine* CreateEngine(IGameInstance* game);
}

#endif //NIGHT_SCAPE_ENGINE_h