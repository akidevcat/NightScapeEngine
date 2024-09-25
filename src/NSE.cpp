#include "NSE.h"

NSE::Engine* NSE::CreateEngine(IAppInstance* app)
{
    return new Engine(app);
}
