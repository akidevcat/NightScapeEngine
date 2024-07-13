#pragma once

#include <cassert>
#include <type_traits>

template<class T>
class GameSystem
{
protected:
    GameSystem();
    virtual ~GameSystem() = 0;

public:
    static T* Get()
    {
        assert(_instance != nullptr);
        return _instance;
    }

    virtual void OnStart() {}
    virtual void OnUpdate() {}

private:
    inline static T* _instance = nullptr;
};

template<class T>
GameSystem<T>::GameSystem()
{
    static_assert(std::is_base_of_v<GameSystem, T>);
    _instance = static_cast<T*>(this);
}

template<class T>
GameSystem<T>::~GameSystem()
{
    _instance = nullptr;
}