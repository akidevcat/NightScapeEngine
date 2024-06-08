#ifndef SHIPCONTROLLER_H
#define SHIPCONTROLLER_H
#include "../engine/entity/VisualMeshEntity.h"

class ShipController : public NSE::VisualMeshEntity
{
public:
    ShipController();
    ~ShipController() override;
};

#endif //SHIPCONTROLLER_H
