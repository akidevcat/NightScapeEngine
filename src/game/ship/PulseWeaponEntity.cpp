#include "PulseWeaponEntity.h"

#include <iostream>

#include "../../engine/servers/TimeServer.h"
#include "../data/ProjectileData.h"

using namespace NSE;

PulseWeaponEntity::PulseWeaponParticles::PulseWeaponParticles() : ParticleSystem(10000, sizeof(Particle), 0, RenderType::Line)
{
    auto shader = NSE::CreateObject<Shader>(L"Assets/Shaders/ParticlePulseWeapon.hlsl");
    shader->Compile();
    renderingMaterial = NSE::CreateObject<Material>(shader);
    renderingMaterial->MakeAdditive();
}

void PulseWeaponEntity::PulseWeaponParticles::SetProjectiles(const std::deque<ProjectileData>* projectiles)
{
    _projectiles = projectiles;
}

void PulseWeaponEntity::PulseWeaponParticles::OnSetupParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);
}

void PulseWeaponEntity::PulseWeaponParticles::OnProcessParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    int i = 0;

    assert(_projectiles->size() <= GetCapacity());

    for (const auto& p : *_projectiles)
    {
        particles[i].position = (float3)(p.position - position);
        particles[i].direction = p.velocity;

        i++;
    }

    SetCount(_projectiles->size());
}

PulseWeaponEntity::PulseWeaponEntity(NSE::Scene* scene)
{
    _particles = scene->Create<PulseWeaponParticles>();
    _particles->SetProjectiles(&_projectiles);
}

void PulseWeaponEntity::Fire()
{
    if (_fireTime > 0.0)
    {
        return;
    }

    auto origin = _isRightTurn ? _rightFirePivot : _leftFirePivot;

    float startVelocity = 500.0f;

    auto pos = DirectX::XMVector3Rotate(XMLoadFloat3(&origin), rotation);
    auto vel = Forward();
    auto projectile = ProjectileData{
        position + pos,
        float3{vel.m128_f32[0] * startVelocity, vel.m128_f32[1] * startVelocity, vel.m128_f32[2] * startVelocity},
        0,
        20.0f};
    _projectiles.emplace_back(projectile);

    _fireTime = _fireTimeout;
    _isRightTurn = !_isRightTurn;
}

void PulseWeaponEntity::SyncPosition()
{
    _particles->position = position;
}

void PulseWeaponEntity::OnUpdate()
{
    auto dt = TimeServer::Get()->Delta();

    if (_fireTime > 0.0)
        _fireTime -= dt;

    auto flDestroyParticle = false;

    for (auto& p : _projectiles)
    {
        p.age += dt;
        auto deltaPos = p.velocity * dt;
        p.position.x += deltaPos.x;
        p.position.y += deltaPos.y;
        p.position.z += deltaPos.z;

        if (p.age > _projectileLifetime)
        {
            flDestroyParticle = true; // Pretty stupid check but 'll do it for now
        }
    }

    if (flDestroyParticle)
    {
        _projectiles.pop_front();
    }


}

void PulseWeaponEntity::SetPivotOffsets(float3 leftPivot, float3 rightPivot)
{
    _leftFirePivot = leftPivot;
    _rightFirePivot = rightPivot;
}
