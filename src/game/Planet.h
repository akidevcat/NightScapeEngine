#ifndef PLANET_H
#define PLANET_H

#include "../engine/entity/VisualMeshEntity.h"

class Planet : public NSE::VisualMeshEntity
{
public:

protected:
    void OnCreated() override;
};

#endif //PLANET_H
