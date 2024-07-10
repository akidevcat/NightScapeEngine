#ifndef SKYBOXSTARSPARTICLES_H
#define SKYBOXSTARSPARTICLES_H

#include "../engine/entity/ParticleSystem.h"

class SkyboxStarsParticles : public NSE::ParticleSystem
{
public:
    struct Particle
    {
        float3 position = {};
        float size = 1;
        float3 color = {1,1,1};
        float padding;
    };

public:
    SkyboxStarsParticles();

protected:
    void OnSetupParticles(void *particlesData, size_t particleCount) override;
    void OnProcessParticles(void *particlesData, size_t particleCount) override;
};

#endif //SKYBOXSTARSPARTICLES_H
