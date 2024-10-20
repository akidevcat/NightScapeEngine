#pragma once

#include "IComponentFactory.h"
#include "../Scene/Scene.h"

namespace NSE
{
    class FactoryRegistry : public RefCounted
    {
    public:
        FactoryRegistry();

        template <typename T>
        void RegisterComponent()
        {
            std::function<void(Entity)> func = [](Entity entity) { entity.AddComponent<T>(); };
            _componentFactories.emplace(typeid(T).hash_code(), func);
        }

        template <typename T>
        void RegisterEntitySystem()
        {
            std::function<void(Ref<Scene>)> func = [](Ref<Scene> scene) { auto system = SRef{new T{}}; scene->RegisterEntitySystem(system); };
            _entitySystemFactories.emplace(typeid(T).hash_code(), func);
        }

    private:
        std::unordered_map<size_t, std::function<void(Entity)>> _componentFactories;
        std::unordered_map<size_t, std::function<void(Ref<Scene>)>> _entitySystemFactories;
    };
}
