#ifndef SKYBOXSTARSPARTICLES_H
#define SKYBOXSTARSPARTICLES_H

#include "../engine/entity/ParticleSystem.h"

class SkyboxStarsParticles : public NSE::ParticleSystem
{
public:
    struct Particle
    {
        float3 position = {};
        float2 size = {1,1};
        float3 color = {1,1,1};
        float4 atlasMapping = {};
    };

public:
    SkyboxStarsParticles();

protected:
    void OnSetupParticles(void *particlesData, size_t particleCount) override;
    void OnProcessParticles(void *particlesData, size_t particleCount) override;
};

#endif //SKYBOXSTARSPARTICLES_H
