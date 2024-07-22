#include "DroidEntity.h"

#include <iostream>

#include "../../engine/servers/AssetServer.h"
#include "../systems/MainSystem.h"

#include "../ShipController.h"

using namespace NSE;

DroidEntity::DroidEntity()
{
    auto shader = CreateObject<Shader>(L"Assets/Shaders/Lit.hlsl");
    shader->Compile();
    auto material = CreateObject<Material>(shader);
    auto tex = AssetsServer::Get()->LoadTextureAsset(L"Assets/Models/APD_Diffuse.dds");
    material->SetTexture(ShaderUtils::PropertyToID("_MainTex"), tex);
    renderingMaterial = material;
    mesh = AssetsServer::Get()->LoadMeshAsset("Assets/Models/APD.obj");
    scale = {3, 3, 3};
    position = {0, 0, 10};
}

void DroidEntity::OnUpdate()
{
    auto time = TimeServer::Get();

    std::vector<obj_ptr<ShipController>> players;
    MainSystem::Get()->GetMainScene()->FindAllEntitiesFromBaseType<ShipController>(players);

    obj_ptr<ShipController> target = nullptr;

    if (!players.empty())
        target = players.front();

    auto targetDistance = length(target->position - position);
    auto targetDirection = (position - target->position + target->Up() * -5.0f) / targetDistance + Vector3d{0.01, 0.01, 0.01};
    auto upVec = Up();
    auto up = float3{upVec.m128_f32[0], upVec.m128_f32[1], upVec.m128_f32[2]};
    xmvector targetRotation;

    if (_isApproachingPhase)
    {
        if (targetDistance < _approachThresholdDistance)
        {
            _isApproachingPhase = false;
        }

        targetRotation = Math::LookRotation((float3)-targetDirection, normalize(up));
    }
    else
    {
        if (targetDistance > _escapeThresholdDistance)
        {
            _isApproachingPhase = true;
        }

        // targetRotation = Math::LookRotation((float3)targetDirection, normalize(up));
        targetRotation = Math::LookRotation((float3)targetDirection, normalize(up));
    }

    // rotation = DirectX::XMQuaternionSlerp(rotation, targetRotation, time->Delta() * 3.0f);

    // position += Forward() * time->Delta() * 20.0f;
}
