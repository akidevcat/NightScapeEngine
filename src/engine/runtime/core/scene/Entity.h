#pragma once

#include "../../../../pch.h"

namespace NSE
{
    class Scene;

    class Entity
    {
    protected:
        Entity();
        ~Entity();


    private:
        entt::entity _ref = entt::null;
        size_t _sceneID = 0;

        friend Scene;
    };
}
