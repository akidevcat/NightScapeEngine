#include "RadarParticleSystem.h"

#include "systems/NavigationSystem.h"
#include "ShipController.h"

using namespace NSE;
using namespace DirectX;

RadarParticleSystem::RadarParticleSystem(const obj_ptr<ShipController>& controller): ParticleSystem(128, sizeof(Particle), 0, RenderType::Point)
{
    _controller = controller;
    auto shader = CreateObject<Shader>(L"Assets/Shaders/RadarParticle.hlsl");
    shader->Compile();
    renderingMaterial = CreateObject<Material>(shader);
    renderingMaterial->MakeTransparent();
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
    auto navTarget = _controller->GetNavTarget();

    auto nav = NavigationSystem::Get();

    auto f = Forward();
    auto forward = float3{f.m128_f32[0], f.m128_f32[1], f.m128_f32[2]};
    forward = normalize(forward);

    int i = 0;
    for (const auto& marker : *nav)
    {
        if (marker == navTarget)
        {
            particles[i].color = {1.00, 0.547, 0.0300, 1.0};
        }
        else
        {
            particles[i].color = marker->GetNavigatableColor();
        }
        // particles[i].color = marker->GetNavigatableColor();
        particles[i].position = (float3)(marker->GetNavigatablePosition() - position);
        auto fDotP = dotproduct(normalize(particles[i].position), forward);
        particles[i].color.w = fDotP * 0.45f + 0.55f;
        i++;
    }

    SetCount(i);
}
