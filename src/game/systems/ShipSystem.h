#pragma once

#include "GameSystem.h"
#include "../../engine/obj_ptr.h"
#include "../ShipController.h"

class ShipSystem : public GameSystem<ShipSystem>
{
public:
    ShipSystem();

    [[nodiscard]] obj_ptr<ShipController> GetShipController() const { return _shipController; }

private:
    obj_ptr<ShipController> _shipController = nullptr;
};
