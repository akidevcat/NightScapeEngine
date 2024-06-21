#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Camera.h"
#include "VisualEntity.h"

namespace NSE
{
    class ParticleSystem : public VisualEntity
    {
    public:
        ParticleSystem() = delete;
        ParticleSystem(int capacity, size_t particlesStrideSize);
        ~ParticleSystem() override;

        virtual void OnProcessParticles() = 0;

        void RenderEntity(const NSE_Camera& camera) override;
        void OnParticlesUpdate();

        void Release();

    public:
        float simulationSpeed = 1.0f;
        int simulationFramerate = 20;

        // RenderMode

        // float   pLifetime = 1.0f;
        // float   pVelocity = 1.0f;
        // float3  pSize = {1,1,1};

    protected:
        int _seed = 0;
        int _capacity = 0;
        void* _particles = nullptr;
    };

}

#endif //PARTICLESYSTEM_H