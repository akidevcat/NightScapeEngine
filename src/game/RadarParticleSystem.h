#ifndef RADARPARTICLESYSTEM_H
#define RADARPARTICLESYSTEM_H

#include "../engine/entity/ParticleSystem.h"

class RadarParticleSystem : public NSE::ParticleSystem
{
public:

    struct Particle
    {
        float3 position;
        float padding;
        float4 color;
    };

public:
    RadarParticleSystem();

    void AddTarget(const NSE_SceneEntity& target, float4 color);
    void RemoveTarget(const NSE_SceneEntity& target);

protected:
    void OnSetupParticles(void *particlesData, size_t particleCount) override;
    void OnProcessParticles(void *particlesData, size_t particleCount) override;

private:
    std::unordered_map<size_t, std::pair<NSE_SceneEntity, float4>> _targets;
};

#endif //RADARPARTICLESYSTEM_H
