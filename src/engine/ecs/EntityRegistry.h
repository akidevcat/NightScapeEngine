#ifndef ENTITYREGISTRY_H
#define ENTITYREGISTRY_H

#include "Entity.h"
#include "../obj_ptr.h"
#include "../servers/ObjectServer.h"

class EntityRegistry
{
public:
    // EntityRegistry();
    // ~EntityRegistry();
    //
    // template <typename... IncludeTypes, typename... ExcludeTypes>
    // void Find()
    // {
    //
    // }
    //
    // template <class T, class... ArgTypes, std::enable_if_t<std::is_base_of_v<Entity, T>, int> = 0>
    // obj_ptr<T> CreateEntity(ArgTypes&&... args)
    // {
    //     std::shared_ptr<T> obj = std::make_shared<T>(std::forward<ArgTypes>(args)...);
    //     auto ent = static_cast<std::shared_ptr<Entity>>(ent);
    //     _objects.emplace(ent->GetUID(), obj);
    //     ent->OnCreated();
    //     return ent;
    // }
    //
    // template <typename Type>
    // void AddComponent()
    // {
    //
    // }
    //
    // template <typename Type>
    // void RemoveComponent()
    // {
    //
    // }

private:

};

#endif //ENTITYREGISTRY_H
