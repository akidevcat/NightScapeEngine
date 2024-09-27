#pragma once

namespace NSE
{
    template<typename T, typename ... TArgs>
    T& Entity::AddComponent(TArgs&& ...args)
    {
        assert(("An attempt to add an existing component", !HasComponent<T>()));
        return _scene->_registry.emplace<T>(_ref, std::forward<TArgs>(args)...);
    }

    template<typename T>
    T& Entity::GetComponent()
    {
        assert(("An attempt to get a non-existing component", HasComponent<T>()));
        return _scene->_registry.get<T>(_ref);
    }

    template<typename T>
    const T& Entity::GetComponent() const
    {
        assert(("An attempt to get a non-existing component", HasComponent<T>()));
        return _scene->_registry.get<T>(_ref);
    }

    template<typename T>
    T* Entity::TryGetComponent()
    {
        assert(("Null reference occured ( Entity->TryGetComponent )", Alive()));
        return _scene->_registry.try_get<T>(_ref);
    }

    template<typename T>
    const T* Entity::TryGetComponent() const
    {
        assert(("Null reference occured ( Entity->TryGetComponent )", Alive()));
        return _scene->_registry.try_get<T>(_ref);
    }

    template<typename ... T>
    bool Entity::HasComponent() const
    {
        assert(("Null reference occured ( Entity->HasComponent )", Alive()));
        return _scene->_registry.all_of<T...>(_ref);
    }

    template<typename ... T>
    bool Entity::HasAny() const
    {
        assert(("Null reference occured ( Entity->HasAny )", Alive()));
        return _scene->_registry.any_of<T...>(_ref);
    }

    template<typename T>
    void Entity::RemoveComponent()
    {
        assert(("An attempt to add an existing component", HasComponent<T>()));
        _scene->_registry.remove<T>(_ref);
    }

    template<typename T>
    bool Entity::TryRemoveComponent()
    {
        assert(("Null reference occured ( Entity->TryRemoveComponent )", Alive()));
        return _scene->_registry.remove<T...>(_ref) > 0;
    }
}