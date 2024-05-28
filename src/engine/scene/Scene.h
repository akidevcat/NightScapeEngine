#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <unordered_map>

#include "../entity/SceneEntity.h"

class Scene
{
public:
    Scene();
    ~Scene();

    size_t GetUID() const { return _uid; }

    void GetAllEntities(std::vector<SceneEntity*>& vec);
    template <typename T> void FindAllEntitiesFromBaseType(std::vector<T*>& vec);
    void RegisterEntity(SceneEntity* entity);
    void UnregisterEntity(SceneEntity* entity);
    void DestroyEntity(SceneEntity* entity);

private:
    size_t _uid;
    static size_t _uidCount;

    std::unordered_map<size_t, std::unordered_map<size_t, SceneEntity*>*> _entities =
        std::unordered_map<size_t, std::unordered_map<size_t, SceneEntity*>*>();
    // unordered_map<size_t, unordered_map<size_t, unique_ptr<SceneEntity>>*> _entities =
    //     unordered_map<size_t, unordered_map<size_t, unique_ptr<SceneEntity>>*>();
};

template <typename T>
void Scene::FindAllEntitiesFromBaseType(std::vector<T*>& vec)
{
    for (auto it : _entities)
    {
        // Check if map is empty
        if (it.second->begin() == it.second->end())
        {
            continue;
        }

        // check if the new type is a child for this type
        if (!dynamic_cast<T*>(it.second->begin()->second))
        {
            continue;
        }

        for (auto eit : *it.second)
        {
            vec.emplace_back(dynamic_cast<T*>(eit.second));
        }
    }
}

#endif //SCENE_H