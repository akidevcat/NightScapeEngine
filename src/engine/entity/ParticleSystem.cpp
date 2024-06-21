#include "ParticleSystem.h"

NSE::ParticleSystem::ParticleSystem(int capacity, size_t particlesStrideSize)
{
    _capacity = capacity;
    _particles = malloc(capacity * particlesStrideSize);
}

NSE::ParticleSystem::~ParticleSystem()
{
    Release();
}

void NSE::ParticleSystem::RenderEntity(const obj_ptr<NSE::Camera> &camera)
{

}

void NSE::ParticleSystem::OnParticlesUpdate()
{

}

void NSE::ParticleSystem::Release()
{
    if (_particles)
    {
        free(_particles);
    }
}
