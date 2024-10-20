#include "NSE.h"

NSE::Engine* NSE::CreateEngine(IAppInstance*&& app, IEditorInstance*&& editor)
{
    return new Engine{app, editor};
}
