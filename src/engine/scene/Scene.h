#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <unordered_map>
#include "../obj_ptr.h"

#include "../entity/SceneEntity.h"
#include "../entity/VisualEntity.h"
#include "../servers/ObjectServer.h"

#define NSE_Scene obj_ptr<NSE::Scene>

namespace NSE
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        [[nodiscard]] size_t GetUID() const { return _uid; }

        void GetAllEntities(std::vector<NSE_SceneEntity>& vec);
        template <typename T, std::enable_if_t<std::is_base_of_v<SceneEntity, T>, int> = 0>
            void FindAllEntitiesFromBaseType(std::vector<obj_ptr<T>>& vec, bool skipDisabled = true);

        template <class T, class... ArgTypes, std::enable_if_t<std::is_base_of_v<SceneEntity, T>, int> = 0>
            obj_ptr<T> Create(ArgTypes&&... args);

        void Destroy(const NSE_SceneEntity& entity);


    private:
        void RegisterEntity(const NSE_SceneEntity& entity);
        void UnregisterEntity(const NSE_SceneEntity& entity);

        size_t _uid;
        static size_t _uidCount;

        std::unordered_map<size_t, std::unordered_map<size_t, NSE_SceneEntity>*> _entities =
            std::unordered_map<size_t, std::unordered_map<size_t, NSE_SceneEntity>*>();
    };

    template <typename T, std::enable_if_t<std::is_base_of_v<SceneEntity, T>, int>>
        void Scene::FindAllEntitiesFromBaseType(std::vector<obj_ptr<T>>& vec, bool skipDisabled)
    {
        for (auto it : _entities)
        {
            // Check if map is empty
            if (it.second->begin() == it.second->end())
            {
                continue;
            }

            // check if the new type is a child for this type
            if (!dynamic_cast<T*>(it.second->begin()->second.get())) // ToDo looks evil
            {
                continue;
            }

            for (const auto& eit : *it.second)
            {
                if (skipDisabled && !eit.second->IsEnabled())
                    continue;

                vec.emplace_back(*reinterpret_cast<obj_ptr<T>*>(const_cast<obj_ptr<SceneEntity>*>(&eit.second))); // This is evil AF
            }
        }
    }

    template <class T, class... ArgTypes, std::enable_if_t<std::is_base_of_v<SceneEntity, T>, int>>
        obj_ptr<T> Scene::Create(ArgTypes&&... args)
    {
        // Transfer creation process to ObjectServer
        obj_ptr<T> result = ObjectServer::Get()->CreateSilently<T>(std::forward<ArgTypes>(args)...);

        // Register created entity on the scene
        RegisterEntity(result);

        static_cast<obj_ptr<SceneEntity>>(result)->OnCreated();

        return result;

        // static_assert(std::is_base_of_v<SceneEntity, T>, "T must inherit from SceneEntity");
        // T* ent = new T(_STD forward<ArgTypes>(args)...);
        // shared_ptr<T> shared_ent = make_shared<T, ArgTypes>(args);
        // std::shared_ptr<T> ent = make_shared<T, ArgTypes>(std::forward<ArgTypes>(args)...);
    }
}

#endif //SCENE_H
