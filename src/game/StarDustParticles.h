#ifndef STARDUSTPARTICLES_H
#define STARDUSTPARTICLES_H

#include "../engine/entity/ParticleSystem.h"

class StarDustParticles : public NSE::ParticleSystem
{
public:
    struct Particle
    {
        float3 position = {};
        float3 direction = {};
        float size = 0.1;
        float alpha = 1;
    };

public:
    StarDustParticles();

public:
    float3 velocity;

protected:
    void OnSetupParticles(void *particlesData, size_t particleCount) override;
    void OnProcessParticles(void *particlesData, size_t particleCount) override;

};

#endif //STARDUSTPARTICLES_H
