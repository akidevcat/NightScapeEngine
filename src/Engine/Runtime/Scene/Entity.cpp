#include "Entity.h"

#include "Scene.h"

bool NSE::Entity::Alive() const
{
    return
        _ref != entt::null &&
        _scene.Alive() &&
        _scene->_registry.valid(_ref);
}
