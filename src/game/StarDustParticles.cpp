#include "StarDustParticles.h"

#include "../engine/math/Math.h"
#include "../engine/servers/TimeServer.h"

using namespace DirectX;
using namespace NSE;

StarDustParticles::StarDustParticles() : ParticleSystem(1000, sizeof(Particle), 1000, RenderType::Line)
{
    auto shader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/ParticleUnlitLine.hlsl");
    shader->Compile();
    renderingMaterial = NSE::CreateObject<NSE::Material>(shader);
}

void StarDustParticles::OnSetupParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    constexpr float r = 50.0f;

    for (size_t i = 0; i < particleCount; i++)
    {
        particles[i].position = float3(NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1);
        particles[i].position = float3((XMVector3Normalize(XMLoadFloat3(&particles[i].position)) * NSE::Math::Random() * r).m128_f32);
        particles[i].direction = float3();
        particles[i].size = 0.03;
    }
}

void StarDustParticles::OnProcessParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);
    auto dt = NSE::TimeServer::Get()->Delta();

    constexpr float r = 50.0f;
    constexpr float r2 = r*r;

    for (size_t i = 0; i < particleCount; i++)
    {
        auto pos = particles[i].position;
        float m2 = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
        if (m2 > r2)
        {
            pos = float3(NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1);
            pos = float3((XMVector3Normalize(XMLoadFloat3(&pos)) * r).m128_f32);
        }
        particles[i].direction = velocity;
        particles[i].direction *= -0.1f;
        if (length(particles[i].direction) < 0.1f)
        {
            particles[i].direction = {};
        }

        pos -= velocity * dt;
        particles[i].position = pos;

        // particles[i].position = float3(NSE::Math::Random(), NSE::Math::Random(), NSE::Math::Random());
        // particles[i].size = 0.1;
    }
}
