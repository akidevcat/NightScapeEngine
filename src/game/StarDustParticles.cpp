#include "StarDustParticles.h"

#include "../engine/math/Math.h"

StarDustParticles::StarDustParticles() : ParticleSystem(10000, sizeof(Particle), 10000, RenderType::Line)
{
    auto shader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/ParticleUnlitLine.hlsl");
    shader->Compile();
    renderingMaterial = NSE::CreateObject<NSE::Material>(shader);
}

void StarDustParticles::OnSetupParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    for (size_t i = 0; i < particleCount; i++)
    {
        particles[i].position = float3(NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1);
        particles[i].direction = float3(NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1);
        particles[i].size = 0.03;
    }
}

void StarDustParticles::OnProcessParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    // for (size_t i = 0; i < particleCount; i++)
    // {
    //     // particles[i].position = float3(NSE::Math::Random(), NSE::Math::Random(), NSE::Math::Random());
    //     // particles[i].size = 0.1;
    // }
}
