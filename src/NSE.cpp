#include "NSE.h"

NSE::Engine* NSE::CreateEngine(IGameInstance* game)
{
    return new Engine(game);
}
