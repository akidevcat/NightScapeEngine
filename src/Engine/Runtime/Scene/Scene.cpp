#include "Scene.h"

#include "Components/Transform.h"
#include "../Render/Components/Camera.h"

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
    return nullptr;
}

void NSE::Scene::DestroyEntity(Entity entity)
{
}

void NSE::Scene::DestroyEntityNow(Entity entity)
{
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
