#pragma once

#include <entt/entt.hpp>
#include "Entity.h"

namespace NSE
{
    class SceneServer;

    class Scene : public RefCounted
    {
    public:
        Scene();
        ~Scene() override;

    protected:
        void OnDispose();

    public:
        [[nodiscard]] size_t GetUID() const { return _uid; }

    public:
        std::string name = "Unknown";

    private:
        size_t _uid;

        entt::registry _registry;

        static size_t _uidCount;

        friend SceneServer;
        friend Entity;
    };
}

// Entity.h requires some template implementations based of the Scene class
#include "Entity_SceneImplementation.h"