#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Camera.h"
#include "VisualEntity.h"
#include "../data/Mesh.h"

namespace NSE
{
    class ParticleSystem : public VisualEntity
    {
    public:
        ParticleSystem() = delete;
        ParticleSystem(size_t capacity, size_t stride);
        ~ParticleSystem() override;

        void RenderEntity(const NSE_Camera& camera) override;
        void OnParticlesUpdate();

        void Release();

        void SetSeed(int seed);
        [[nodiscard]] int GetSeed() const { return _seed; }

        void SetCapacity(size_t capacity);
        [[nodiscard]] size_t GetCapacity() const { return _capacity; }

        [[nodiscard]] size_t GetCount() const { return _count; }

        void SetSimulationFramerate(int framerate);
        [[nodiscard]] int GetSimulationFramerate() const { return _simulationFramerate; }

        void SetParticleMesh(const NSE_Mesh& mesh);

    public:

    protected:
        void Initialize();

        void SetCount(size_t count);

        virtual void OnProcessParticles(void* particlesData, size_t particleCount) = 0;

    private:
        int             _seed = 0;
        size_t          _capacity = 0;
        size_t          _stride = 0;
        size_t          _count = 0;
        size_t          _simulatedFrameCount = 0;
        float           _simulationTime = 0.0f;
        GraphicsBuffer  _particlesDataBuffer;
        NSE_Mesh        _pariclesMesh = nullptr;
        int             _simulationFramerate = 0;

        static constexpr int   MAX_SIMULATION_FRAMES_PER_UPDATE = 3;
    };

}

typedef obj_ptr<NSE::ParticleSystem> NSE_ParticleSystem;

#endif //PARTICLESYSTEM_H