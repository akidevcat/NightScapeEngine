#pragma once

#include <queue>

#include "WeaponEntity.h"
#include "../../engine/entity/ParticleSystem.h"
#include "../data/ProjectileData.h"

class PulseWeaponEntity : public WeaponEntity
{
public:
    class PulseWeaponParticles : public NSE::ParticleSystem
    {
    public:
        struct Particle
        {
            float3 position = {};
            float3 direction = {};
            float2 color = {1,1};
        };

    public:
        PulseWeaponParticles();

        void SetProjectiles(const std::deque<ProjectileData>* projectiles);

    protected:
        void OnSetupParticles(void *particlesData, size_t particleCount) override;
        void OnProcessParticles(void *particlesData, size_t particleCount) override;

    private:
        const std::deque<ProjectileData>* _projectiles = nullptr;
    };

public:
    PulseWeaponEntity(NSE::Scene* scene);

    void Fire();
    void SyncPosition();
    void OnUpdate() override;

    void SetPivotOffsets(float3 leftPivot, float3 rightPivot);

private:
    float3 _leftFirePivot{};
    float3 _rightFirePivot{};

    bool _isRightTurn = true;
    float _fireTime = 0;
    float _fireTimeout = 0.05f;
    float _projectileEnergy = 10.0f;
    float _projectileLifetime = 10.0f;

    obj_ptr<PulseWeaponParticles> _particles = nullptr;

    std::deque<ProjectileData> _projectiles;
};
