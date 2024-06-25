#ifndef STARDUSTPARTICLES_H
#define STARDUSTPARTICLES_H

#include "../engine/entity/ParticleSystem.h"

class StarDustParticles : public NSE::ParticleSystem
{
public:
    struct Particle
    {
        float3 position = {};
        float size = 0;
    };

public:
    StarDustParticles();

protected:
    void OnProcessParticles(void *particlesData, size_t particleCount) override;

};

#endif //STARDUSTPARTICLES_H
