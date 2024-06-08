#ifndef SHIPRADARCONTROLLER_H
#define SHIPRADARCONTROLLER_H

#include "../engine/NightScapeEngine.h"

class ShipRadarController : public NSE::VisualMeshEntity
{
public:
    ShipRadarController();
    ~ShipRadarController();

    void OnUpdate() override;
};



#endif //SHIPRADARCONTROLLER_H
