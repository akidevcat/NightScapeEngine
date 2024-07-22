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

    obj_ptr<INavigatable> FindAlignedNavigatable(NSE::Vector3d origin, NSE::Vector3d direction, double cosMin, bool isShiftSpaceTarget = false);

    [[nodiscard]] std::vector<obj_ptr<INavigatable>>::const_iterator begin() const { return _targets.begin(); }
    [[nodiscard]] std::vector<obj_ptr<INavigatable>>::const_iterator end() const { return _targets.end(); }
    [[nodiscard]] std::vector<obj_ptr<INavigatable>>::const_iterator cbegin() const { return _targets.cbegin(); }
    [[nodiscard]] std::vector<obj_ptr<INavigatable>>::const_iterator cend() const { return _targets.cend(); }

private:
    std::vector<obj_ptr<INavigatable>> _targets;
};
