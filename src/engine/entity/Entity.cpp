#include "Entity.h"

size_t Entity::_uidCount = 0;

Entity::Entity()
{
    _uid = _uidCount++;
}

Entity::~Entity()
{

}

void Entity::Create(Entity *entity)
{

}

size_t Entity::GetUID()
{
    return _uid;
}



