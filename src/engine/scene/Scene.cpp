#include "Scene.h"

#include <typeinfo>

#include "../entity/SceneEntity.h"

using namespace std;

size_t Scene::_uidCount = 0;

Scene::Scene()
{
    _uid = _uidCount++;
}

Scene::~Scene()
{
    // Deallocate maps
    for (auto typePair : _entities)
    {
        for (auto entityPair : *typePair.second)
        {
            delete entityPair.second;
        }

        delete typePair.second;
    }
}

void Scene::GetAllEntities(vector<SceneEntity*>& vec)
{
    for (auto it : _entities)
    {
        for (auto eit : *it.second)
        {
            vec.emplace_back(eit.second);
        }
    }
}

void Scene::RegisterEntity(SceneEntity *entity)
{
    // Get entity type
    size_t typeId = typeid(*entity).hash_code();

    // Check if type map definition already exists
    auto mit = _entities.find(typeId);
    unordered_map<size_t, SceneEntity*>* typeMap;
    if (mit == _entities.end())
    {
        // Create map
        typeMap = new unordered_map<size_t, SceneEntity*>{};
        _entities.emplace(make_pair(typeId, typeMap));
    }
    else
    {
        // Get the existing one
        typeMap = mit->second;
    }

    typeMap->try_emplace(entity->GetUID(), entity);
    entity->SetSceneUID(GetUID());
}

void Scene::UnregisterEntity(SceneEntity *entity)
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
    typeMap->erase(eit);
}

void Scene::DestroyEntity(SceneEntity *entity)
{
    UnregisterEntity(entity);
    delete entity;
}


