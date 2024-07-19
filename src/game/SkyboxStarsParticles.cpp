#include "SkyboxStarsParticles.h"

#include "../engine/math/Math.h"
#include "../engine/servers/AssetServer.h"
#include "../engine/servers/TimeServer.h"

using namespace DirectX;
using namespace NSE;

SkyboxStarsParticles::SkyboxStarsParticles() : ParticleSystem(1000, sizeof(Particle), 1000, RenderType::Billboard)
{
    auto shader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/ParticleStar.hlsl");
    shader->Compile();
    auto tex = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/SkyboxStars_Atlas.dds");
    renderingMaterial = NSE::CreateObject<NSE::Material>(shader);
    renderingMaterial->MakeAdditive();
    renderingMaterial->renderQueue = Material::RenderQueue::RENDER_QUEUE_BACKGROUND + 1;
    renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    renderingMaterial->SetTexture(ShaderUtils::PropertyToID("_StarsTexture"), tex);
}

void SkyboxStarsParticles::OnSetupParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    constexpr float r = 50.0f;

    for (size_t i = 0; i < particleCount; i++)
    {
        particles[i].position = normalize(float3(NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1, NSE::Math::Random() * 2.0f - 1)) * 100.0f;

        auto spriteIndex = Math::Random();
        if (spriteIndex < 0.7f)
        {
            particles[i].size = {1,1};
            particles[i].atlasMapping = {8.0f / 32.0f, 0.0f, 1.0f / 32.0f, 1.0f / 32.0f};
        }
        else if (spriteIndex < 0.9f)
        {
            particles[i].size = {3,3};
            particles[i].atlasMapping = {5.0f / 32.0f, 0.0f, 3.0f / 32.0f, 3.0f / 32.0f};
        }
        else if (spriteIndex < 1.0f)
        {
            particles[i].size = {5,11};
            particles[i].atlasMapping = {0.0f / 32.0f, 0.0f, 5.0f / 32.0f, 11.0f / 32.0f};
        }
        else if (spriteIndex < 1.0f)
        {
            particles[i].size = {21,21};
            particles[i].atlasMapping = {10.0f / 32.0f, 0.0f, 21.0f / 32.0f, 21.0f / 32.0f};
        }

        float rnd = Math::Random();
        float r = (Math::Random() * 2.0f - 1.0f) * 0.0f;
        float g = (Math::Random() * 2.0f - 1.0f) * 0.0f;
        float b = (Math::Random() * 2.0f - 1.0f) * 0.0f;
        if (rnd < 0.3f)
        {
            particles[i].color = {1 + r, 0.216f + g, 0.427f + b};
        }
        else if (rnd < 0.6f)
        {
            particles[i].color = {0.569f + r, 0.455f + g, 0.988f + b};
        }
        else
        {
            particles[i].color = {0.855f + r, 0.502f + g, 1 + b};
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
