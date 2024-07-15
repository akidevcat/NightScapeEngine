#include "Scene.h"

#include <typeinfo>
#include <utility>

#include "../obj_ptr.h"
#include "../entity/SceneEntity.h"

using namespace std;

size_t NSE::Scene::_uidCount = 0;

NSE::Scene::Scene()
{
    _uid = _uidCount++;
}

NSE::Scene::~Scene()
{
    // Deallocate maps
    for (auto typePair : _entities)
    {
        // for (auto entityPair : *typePair.second)
        // {
        //     // delete entityPair.second;
        //     entityPair.second.reset();
        // }

        delete typePair.second;
    }
}

void NSE::Scene::GetAllEntities(vector<obj_ptr<SceneEntity>>& vec)
{
    for (auto it : _entities)
    {
        for (const auto& eit : *it.second)
        {
            vec.emplace_back(eit.second);
        }
    }
}

void NSE::Scene::RegisterEntity(const NSE_SceneEntity& entity)
{
    // Get entity type
    size_t typeId = typeid(*entity).hash_code();

    // Check if type map definition already exists
    auto mit = _entities.find(typeId);
    unordered_map<size_t, NSE_SceneEntity>* typeMap;
    if (mit == _entities.end())
    {
        // Create map
        typeMap = new unordered_map<size_t, NSE_SceneEntity>{};
        _entities.emplace(make_pair(typeId, typeMap));
    }
    else
    {
        // Get the existing one
        typeMap = mit->second;
    }

    typeMap->try_emplace(entity->GetUID(), entity);
    entity->SetSceneUID(GetUID());

    // Refresh Lookups
    RefreshLookups();
}

void NSE::Scene::UnregisterEntity(const NSE_SceneEntity& entity)
{
    // Get entity type
    size_t typeId = typeid(*entity).hash_code();
    entity->ResetSceneUID();

    // Check if type map definition already exists
    auto mit = _entities.find(typeId);
    if (mit == _entities.end())
    {
        return;
    }

    auto typeMap = mit->second;

    auto eit = typeMap->find(entity->GetUID());
    if (eit == typeMap->end())
    {
        // entity is not registered
        return;
    }

    // Unregister entity
    eit->second->ResetSceneUID();
    typeMap->erase(eit);

    // Refresh Lookups
    RefreshLookups();
}

void NSE::Scene::RefreshLookups()
{
    // _entitiesByTypeLookup.clear();
}

void NSE::Scene::Destroy(const NSE_SceneEntity& entity)
{
    // Firstly unregister this entity from scene
    UnregisterEntity(entity);

    // Now call destroy
    ObjectServer::Get()->DestroyAny(entity);
}
