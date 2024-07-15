#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <typeindex>
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
        // template <typename T, std::enable_if_t<std::is_base_of_v<SceneEntity, T>, int> = 0>
        template <typename T>
            void FindAllEntitiesFromBaseType(std::vector<obj_ptr<T>>& vec, bool skipDisabled = true);

        template <class T, class... ArgTypes, std::enable_if_t<std::is_base_of_v<SceneEntity, T>, int> = 0>
            obj_ptr<T> Create(ArgTypes&&... args);

        void Destroy(const NSE_SceneEntity& entity);


    private:
        using TypeID = size_t;
        using EntityID = size_t;

        template <typename T>
        struct TypeTreeNode
        {
            using type = T;
            std::unordered_map<EntityID, NSE_SceneEntity> entitiesMap;
            std::vector<NSE_SceneEntity> entitiesList;
            std::unordered_map<TypeID, TypeTreeNode> childNodes;

            void Add(const NSE_SceneEntity& entity)
            {
                entitiesMap.emplace(entity->GetUID(), entity);
                entitiesList.emplace_back(entity);
            }

            void Remove(const NSE_SceneEntity& entity)
            {
                entitiesMap.erase(entity->GetUID());
                int i = 0;
                for (; i < entitiesList.size(); i++)
                {
                    if (entitiesList[i] == entity)
                        break;
                }
                entitiesList.erase(entitiesList.begin() + i);
            }

            // void AddChild(TypeTreeNode* childNode)
            // {
            //     childNodes.emplace(childNode->typeID, childNode);
            // }
        };

        void RegisterEntity(const NSE_SceneEntity& entity);
        void UnregisterEntity(const NSE_SceneEntity& entity);
        void RefreshLookups();

        size_t _uid;
        static size_t _uidCount;

        // TypeTreeNode<SceneEntity> _entityTree;
        // std::unordered_map<TypeID, TypeTreeNode<SceneEntity>*> _entityTreeNodes;

        // std::unordered_map<size_t, NSE_SceneEntity> _entities =
        //     std::unordered_map<size_t, NSE_SceneEntity>();
        std::unordered_map<size_t, std::unordered_map<size_t, NSE_SceneEntity>*> _entities =
            std::unordered_map<size_t, std::unordered_map<size_t, NSE_SceneEntity>*>();
    };

    // template <typename T, std::enable_if_t<std::is_base_of_v<SceneEntity, T>, int>>
    template <typename T>
        void Scene::FindAllEntitiesFromBaseType(std::vector<obj_ptr<T>>& vec, bool skipDisabled)
    {
        // size_t typeId = typeid(T);
        //
        // auto nodeIt = _entityTreeNodes.find(typeId);
        // if (nodeIt == _entityTreeNodes.end())
        // {
        //     // Node missing in the tree structure
        //     TypeTreeNode<SceneEntity>* node = &_entityTree;
        //
        //     TypeTreeNode<SceneEntity>* derivedNode = nullptr;
        //
        //     for (auto& childNode : node->childNodes)
        //     {
        //
        //         childNode.second = TypeTreeNode<SceneEntity>{};
        //         childNode.second = TypeTreeNode<T>{};
        //
        //
        //         childNode.second::type;
        //
        //         // assert(!childNode.second->entitiesList.empty());
        //         // void* childElementPtr = childNode.second->entitiesList.front().get();
        //
        //         // T* castResultPtr = dynamic_cast<T>(childNode.second->metadataObject);
        //         // if (!castResultPtr)
        //         //     continue;
        //
        //
        //
        //         // typeid(T).before()
        //     }
        //
        // }

        // auto resultIt = _entitiesByTypeLookup.find(typeId);
        // if (resultIt == _entitiesByTypeLookup.end())
        // {
        //     // Create lookup
        //     resultIt = _entitiesByTypeLookup.emplace(typeId, std::vector<NSE_SceneEntity>{}).first;
        //     const auto& entities = resultIt->second;
        //
        //     for (const auto& it : _entities)
        //     {
        //         T* rawPtr = dynamic_cast<T*>(it.second.get());
        //
        //     }
        // }



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

                // const_cast<obj_ptr<SceneEntity>*>(&eit.second);

                // std::dynamic_pointer_cast<>()

                // dynamic_cast<T*>(eit.second.get());
                // vec.emplace_back(obj_ptr<T>(eit.second));
                // vec.emplace_back(dynamic_pointer_cast<T>(eit.second));

                // vec.emplace_back(obj_ptr<T>(eit.second, dynamic_cast<T*>(eit.second.get())));
                vec.emplace_back(dynamic_pointer_cast<T>(eit.second));

                // std::dynamic_pointer_cast<>()

                // std::dynamic_pointer_cast<>()

                // ;
                // vec.emplace_back(*reinterpret_cast<obj_ptr<T>*>(const_cast<obj_ptr<SceneEntity>*>(&eit.second))); // This is evil AF
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
    }
}

#endif //SCENE_H
