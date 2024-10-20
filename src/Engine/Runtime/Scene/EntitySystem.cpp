#include "EntitySystem.h"

NSE::EntitySystem::EntitySystem()
{

}

void NSE::EntitySystem::Enable()
{
    _isEnabled = true;
    OnEnabled();
}

void NSE::EntitySystem::Disable()
{
    _isEnabled = false;
    OnDisabled();
}
