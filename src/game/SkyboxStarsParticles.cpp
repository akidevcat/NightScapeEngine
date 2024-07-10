#include "SkyboxStarsParticles.h"

#include "../engine/math/Math.h"
#include "../engine/servers/TimeServer.h"

using namespace DirectX;
using namespace NSE;

SkyboxStarsParticles::SkyboxStarsParticles() : ParticleSystem(1000, sizeof(Particle), 1000, RenderType::Billboard)
{
    auto shader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/ParticleStar.hlsl");
    shader->Compile();
    renderingMaterial = NSE::CreateObject<NSE::Material>(shader);
    renderingMaterial->MakeAdditive();
    renderingMaterial->renderQueue = Material::RenderQueue::RENDER_QUEUE_BACKGROUND + 1;
    renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
}

void SkyboxStarsParticles::OnSetupParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    constexpr float r = 50.0f;

    for (size_t i = 0; i < particleCount; i++)
    {
        particles[i].position = normalize(float3(NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1)) * 100.0f;
        particles[i].size = 1 + ceilf(powf(Math::Random(), 2.0f) * 2) * 2;

        float rnd = Math::Random();
        if (rnd < 0.3f)
        {
            particles[i].color = {1, 0.216, 0.427};
        }
        else if (rnd < 0.6f)
        {
            particles[i].color = {0.569, 0.455, 0.988};
        }
        else
        {
            particles[i].color = {0.855, 0.502, 1};
        }
    }
}

void SkyboxStarsParticles::OnProcessParticles(void *particlesData, size_t particleCount)
{

}

// void SkyboxStarsParticles::OnProcessParticles(void *particlesData, size_t particleCount)
// {
//     auto particles = static_cast<Particle*>(particlesData);
//     auto dt = NSE::TimeServer::Get()->Delta();
//
//     constexpr float r = 50.0f;
//     constexpr float r2 = r*r;
//
//     for (size_t i = 0; i < particleCount; i++)
//     {
//         auto pos = particles[i].position;
//         float m2 = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
//         if (m2 > r2)
//         {
//             pos = float3(NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1);
//             pos = float3((XMVector3Normalize(XMLoadFloat3(&pos)) * r).m128_f32);
//         }
//         particles[i].direction = velocity;
//         particles[i].direction *= -0.1f;
//         if (length(particles[i].direction) < 0.1f)
//         {
//             particles[i].direction = {};
//         }
//
//         pos -= velocity * dt;
//         particles[i].position = pos;
//     }
// }
