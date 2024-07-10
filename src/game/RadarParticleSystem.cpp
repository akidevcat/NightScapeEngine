#include "RadarParticleSystem.h"

using namespace NSE;
using namespace DirectX;

RadarParticleSystem::RadarParticleSystem(): ParticleSystem(128, sizeof(Particle), 0, RenderType::Point)
{
    auto shader = CreateObject<Shader>(L"Assets/Shaders/RadarParticle.hlsl");
    shader->Compile();
    renderingMaterial = CreateObject<Material>(shader);
    renderingMaterial->renderQueue = Material::RENDER_QUEUE_OVERLAY;
}

void RadarParticleSystem::AddTarget(const NSE_SceneEntity& target, float4 color)
{
    _targets.try_emplace(target->GetUID(), std::make_pair(target, color));
}

void RadarParticleSystem::RemoveTarget(const NSE_SceneEntity& target)
{
    _targets.erase(target->GetUID());
}

void RadarParticleSystem::OnSetupParticles(void *particlesData, size_t particleCount)
{

}

void RadarParticleSystem::OnProcessParticles(void *particlesData, size_t particleCount)
{
    auto particles = static_cast<Particle*>(particlesData);

    int i = 0;
    for (const auto& target : _targets)
    {
        if (!target.second.first)
            continue;

        particles[i].color = target.second.second;
        particles[i].position = (float3)(target.second.first->position - position);
        i++;
    }

    SetCount(i);
}
