#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>

#include "../entity/SceneEntity.h"

using namespace std;

class Scene
{
public:
    Scene();
    ~Scene();

    size_t GetUID() const { return _uid; }

    void
        GetAllEntities(vector<SceneEntity*>& vec);
    template <typename T> void
        FindAllEntitiesFromBaseType(vector<T*>& vec);
    void
        RegisterEntity(SceneEntity* entity);
    void
        UnregisterEntity(SceneEntity* entity);

private:
    size_t _uid;
    static size_t _uidCount;

    unordered_map<size_t, unordered_map<size_t, SceneEntity*>*> _entities =
        unordered_map<size_t, unordered_map<size_t, SceneEntity*>*>();
};

template <typename T>
void Scene::FindAllEntitiesFromBaseType(vector<T*>& vec)
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