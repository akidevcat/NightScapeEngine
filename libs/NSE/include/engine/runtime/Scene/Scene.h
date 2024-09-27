#pragma once

#include <entt/entt.hpp>
#include "Entity.h"
#include "EntitySystem.h"

namespace NSE
{
    class SceneServer;

    class Scene : public RefCounted
    {
    protected:
        Scene();
    public:
        ~Scene() override;

    protected:
        void OnBeginFrameUpdate();
        void OnFrameUpdate();
        void OnEndFrameUpdate();
        void OnUnload();

    public:
        Entity CreateEntity(const std::string& name = "New Entity", Entity parent = nullptr);
        void DestroyEntity(Entity entity);
        void DestroyEntityNow(Entity entity);

        template<typename... TComponents>
        auto GetEntities()
        {
            return _registry.view<TComponents...>();
        }

        void RegisterEntitySystem(const SRef<EntitySystem>& system);
        void UnregisterEntitySystem(const SRef<EntitySystem>& system);

    public:
        [[nodiscard]] size_t GetUID() const { return _uid; }

        void SetName(const std::string& name) { _name = name; }
        const std::string& GetName() const { return _name; }

    private:
        size_t _uid;
        std::string _name = "New Scene";

        entt::registry _registry;

        std::unordered_map<void*, SRef<EntitySystem>> _systems;
        std::vector<SRef<EntitySystem>> _systemsToBeInitialized;
        std::vector<SRef<EntitySystem>> _systemsToBeDestroyed;

        static size_t _uidCount;

        friend SceneServer;
        friend Entity;
    };
}

// Entity.h requires some template implementations based of the Scene class
#include "Entity_SceneImplementation.h"