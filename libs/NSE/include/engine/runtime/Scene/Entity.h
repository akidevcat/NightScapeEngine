#pragma once

#include "Components/Identity.h"
#include "Components/Transform.h"

namespace NSE
{
    class Scene;

    class Entity
    {
    public:
        Entity() = default;
        Entity(nullptr_t) : Entity() {}
        ~Entity() = default;

        Entity(const entt::entity& ref, const Ref<Scene>& scene) : _ref(ref), _scene(scene) {}

        [[nodiscard]] bool Alive() const;

        template<typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args);

        template<typename T>
        T& GetComponent();

        template<typename T>
        [[nodiscard]] const T& GetComponent() const;

        template<typename T>
        T* TryGetComponent();

        template<typename T>
        [[nodiscard]] const T* TryGetComponent() const;

        template<typename... T>
        [[nodiscard]] bool HasComponent() const;

        template<typename...T>
        [[nodiscard]] bool HasAny() const;

        template<typename T>
        void RemoveComponent();

        template<typename T>
        bool TryRemoveComponent();

        [[nodiscard]] operator bool() const { return Alive(); }
        [[nodiscard]] operator entt::entity() const { return _ref; }

        bool operator==(const Entity& other) const
        {
            return _ref == other._ref && _scene == other._scene;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

        Components::Identity& identity() { return GetComponent<Components::Identity>(); }
        [[nodiscard]] const Components::Identity& identity() const { return GetComponent<Components::Identity>(); }

        Components::Transform& transform() { return GetComponent<Components::Transform>(); }
        [[nodiscard]] const Components::Transform& transform() const { return GetComponent<Components::Transform>(); }

        std::string& name() { return GetComponent<Components::Identity>().name; }
        [[nodiscard]] const std::string& name() const { return GetComponent<Components::Identity>().name; }

        [[nodiscard]] Ref<Scene> scene() const { return _scene; }

    private:
        entt::entity _ref = entt::null;
        Ref<Scene> _scene = nullptr;

        friend Scene;
    };
}
