#pragma once

#include "../../../../pch.h"

namespace NSE
{
    class SceneServer;

    class Scene
    {
    protected:
        Scene();
        ~Scene();

        void OnDispose();

    public:
        [[nodiscard]] size_t GetUID() const { return _uid; }

    private:
        size_t _uid;
        static size_t _uidCount;

        entt::registry _registry;

        friend SceneServer;
    };
}
