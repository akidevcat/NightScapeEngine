#pragma once

#include "../engine/entity/SpriteVisual.h"

class ShipMarkersVisual : public NSE::SpriteVisual
{
public:
    ShipMarkersVisual();

protected:
    void RenderEntity(const obj_ptr<NSE::Camera> &camera) override;
};
