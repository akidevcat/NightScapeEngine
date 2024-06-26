#ifndef SHIPCONTROLLER_H
#define SHIPCONTROLLER_H
#include "ShipRadarController.h"
#include "../engine/data/Light.h"
#include "../engine/entity/VisualMeshEntity.h"

class ShipController : public NSE::VisualMeshEntity
{
public:
    explicit ShipController(NSE::Scene* scene, float screenAspect);
    ~ShipController() override;

    void OnUpdate() override;

    NSE_Camera GetCamera() { return _camera; }

    [[nodiscard]] NSE::Vector3d GetShipVelocity() const { return _shipVelocity; }

public:
    float shipSpeed = 3.0f;
    float shipRotationSpeed = 1.0f;

private:
    float _camMomentumX = 0;
    float _camMomentumY = 0;
    float _camMomentumR = 0;
    float _speedMomentum = 0;
    NSE::Vector3d _shipVelocity = {};

    NSE_Camera _camera = nullptr;
    NSE_Light _cockpitLight = nullptr;
    obj_ptr<ShipRadarController> _shipRadar = nullptr;
};

#endif //SHIPCONTROLLER_H
