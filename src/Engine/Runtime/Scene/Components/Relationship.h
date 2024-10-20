#pragma once

#include "entt/entt.hpp"

namespace NSE::Components
{
    struct Relationship
    {
        entt::entity parent = entt::null;
        std::vector<entt::entity> children;
    };
}
