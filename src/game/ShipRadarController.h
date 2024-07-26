#ifndef SHIPRADARCONTROLLER_H
#define SHIPRADARCONTROLLER_H

#include "RadarParticleSystem.h"
#include "../engine/NightScapeEngine.h"

class ShipRadarController : public NSE::VisualMeshEntity
{
public:
    ShipRadarController(const obj_ptr<ShipController>& controller);
    ~ShipRadarController();

    void OnUpdate() override;

    void AddTarget(const NSE_SceneEntity& target, float4 color);
    void RemoveTarget(const NSE_SceneEntity& target);

    obj_ptr<RadarParticleSystem> GetParticleSystem() { return _pSystem; }

private:
    obj_ptr<RadarParticleSystem> _pSystem = nullptr;
};



#endif //SHIPRADARCONTROLLER_H
