#include "StarDustParticles.h"

#include "../engine/math/Math.h"

StarDustParticles::StarDustParticles() : ParticleSystem(100, sizeof(Particle), 100)
{
    auto shader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/ParticleUnlit.hlsl");
    shader->Compile();
    renderingMaterial = NSE::CreateObject<NSE::Material>(shader);
}

void StarDustParticles::OnSetupParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    for (size_t i = 0; i < particleCount; i++)
    {
        particles[i].position = float3(NSE::Math::Random() * 2 - 1, NSE::Math::Random() * 2 - 1, NSE::Math::Random() * 2 - 1);
        particles[i].size = 0.1;
    }
}

void StarDustParticles::OnProcessParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    for (size_t i = 0; i < particleCount; i++)
    {
        // particles[i].position = float3(NSE::Math::Random(), NSE::Math::Random(), NSE::Math::Random());
        // particles[i].size = 0.1;
    }
}
