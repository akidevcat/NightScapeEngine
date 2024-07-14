#pragma once

#include <unordered_map>
#include <unordered_set>

#include "GameSystem.h"
#include "../../engine/obj_ptr.h"
#include "../data/INavigatable.h"

class NavigationSystem : public GameSystem<NavigationSystem>
{
public:
    void OnUpdate() override;

};
