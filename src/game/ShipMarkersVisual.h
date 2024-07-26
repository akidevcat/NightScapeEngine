#pragma once

#include "../engine/entity/SpriteVisual.h"

class ShipController;

class ShipMarkersVisual : public NSE::SpriteVisual
{
public:
    explicit ShipMarkersVisual(const obj_ptr<ShipController>& controller);

protected:
    void RenderEntity(const obj_ptr<NSE::Camera> &camera) override;

private:
    obj_ptr<ShipController> _controller = nullptr;
};
