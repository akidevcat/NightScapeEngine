#include "Scene.h"

#include "Components/Transform.h"
#include "../Render/Components/Camera.h"
#include "Components/Relationship.h"

size_t NSE::Scene::_uidCount = 1;

NSE::Scene::Scene()
{
    _uid = _uidCount++;
}

NSE::Scene::~Scene()
{

}

void NSE::Scene::OnBeginFrameUpdate()
{
    for (auto& system : _systemsToBeInitialized)
    {
        system->OnSetup();
    }

    _systemsToBeInitialized.clear();
}

void NSE::Scene::OnFrameUpdate()
{
    for (auto& [ptr, system] : _systems)
    {
        system->OnUpdate();
    }
}

void NSE::Scene::OnEndFrameUpdate()
{
    // Destroy entities
    for (auto& entity : _entitiesToBeDestroyed)
    {
        _registry.destroy(entity);
    }

    _entitiesToBeDestroyed.clear();

    // Unload systems
    for (auto& system : _systemsToBeDestroyed)
    {
        system->OnDispose();
    }

    _systemsToBeDestroyed.clear();
}

void NSE::Scene::OnUnload()
{

}

NSE::Entity NSE::Scene::CreateEntity(const std::string& name, Entity parent)
{
    auto ref = _registry.create();
    auto result = GetEntity(ref);
    result.AddComponent<Components::Identity>().name = name;
    result.AddComponent<Components::Transform>();
    result.AddComponent<Components::Relationship>().parent = parent;
    return result;
}

void NSE::Scene::DestroyEntity(Entity entity)
{
    assert(("An attempt to destroy a null entity", entity.Alive()));
    _entitiesToBeDestroyed.emplace_back(entity);
}

void NSE::Scene::DestroyEntityNow(Entity entity)
{
    assert(("An attempt to destroy a null entity", entity.Alive()));
    _registry.destroy(entity);
}

void NSE::Scene::RegisterEntitySystem(const SRef<EntitySystem>& system)
{
    auto ptr = const_cast<EntitySystem*>(system.Raw());
    assert(("Unable to register an entity system: it's already registered", _systems.find(ptr) == _systems.end()));
    assert(("Unable to register an entity system: it's a null reference", system == nullptr));
    _systems.emplace(ptr, system);
}

void NSE::Scene::UnregisterEntitySystem(const SRef<EntitySystem>& system)
{
    auto ptr = const_cast<EntitySystem*>(system.Raw());
    assert(("Unable to unregister an entity system: it's not registered", _systems.find(ptr) != _systems.end()));
    assert(("Unable to unregister an entity system: it's a null reference", system == nullptr));
    _systemsToBeDestroyed.emplace_back(system);
    _systems.erase(ptr);
}
