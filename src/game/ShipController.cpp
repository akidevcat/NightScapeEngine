#include "ShipController.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>

#include "../engine/entity/QuadVisual.h"
#include "../engine/servers/AssetServer.h"
#include "../engine/servers/ObjectServer.h"
#include "../engine/servers/TimeServer.h"
#include "../engine/servers/InputServer.h"
#include "systems/NavigationSystem.h"

using namespace DirectX;
using namespace NSE;

ShipController::ShipController(NSE::Scene* scene, float screenAspect)
{
    auto atlasUITexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Atlas.dds");

    mesh = NSE::AssetsServer::Get()->LoadMeshAsset("Assets/Models/Cockpit.obj");
    auto renderingShader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/Base.hlsl");
    renderingShader->Compile();
    renderingMaterial = NSE::CreateObject<NSE::Material>(renderingShader);

    auto tex = NSE::AssetsServer::Get()->LoadTextureAsset(L"Assets/Models/T_Cockpit_Diffuse.dds");
    renderingMaterial->SetTexture(NSE::ShaderUtils::PropertyToID("_MainTex"), tex);

    auto testQuad = scene->Create<NSE::QuadVisual>();
    testQuad->renderingMaterial = renderingMaterial;
    testQuad->position = {0, 0, 10};

    _camera = scene->Create<NSE::Camera>();
    _camera->targetScene = scene;
    _camera->SetParams(screenAspect, 60.0f, 0.1f, 1000.0f, false, 0.0f);
    _camera->position = {0, 0, -1};
    _camera->clearMode = NSE::CAMERA_CLEAR_MODE_DEPTH;

    _cockpitLight = scene->Create<NSE::Light>();
    _cockpitLight->lightColor = {0.87f, 0.27f, 0.02f, 0.0f};
    _cockpitLight->lightIntensity = 0.25;

    _integrityBar = scene->Create<ProgressBarVisual>();
    _integrityBar->color = {1, 1, 1, 1};
    _integrityBar->foregroundColor = {0.6, 0.6, 0.6, 1.0f};
    _integrityBar->backgroundColor = {0.2, 0.2, 0.2, 0.25f};
    // _integrityBar->renderingMaterial->MakeAdditive();
    _integrityBar->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _integrityBar->renderingMaterial->renderQueue = Material::RenderQueue::RENDER_QUEUE_OVERLAY;
    _integrityBar->progress = 0.8f;
    _integrityBar->invertY = true;
    _integrityBar->sprite.SetRectPixel(12, 26, 43, 13);

    _fuelBar = scene->Create<ProgressBarVisual>();
    _fuelBar->color = {1, 1, 1, 1};
    _fuelBar->foregroundColor = {0.8, 0.25, 0.0, 1.0f};
    _fuelBar->backgroundColor = {0.8 / 3.0, 0.25 / 3.0, 0.0, 0.25f};
    // _fuelBar->renderingMaterial->MakeAdditive();
    _fuelBar->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _fuelBar->renderingMaterial->renderQueue = Material::RenderQueue::RENDER_QUEUE_OVERLAY;
    _fuelBar->progress = 0.8f;
    _fuelBar->invertY = true;
    _fuelBar->sprite.SetRectPixel(12, 0, 43, 13);
    _fuelBar->SetEnabled(false);

    _exposureBar = scene->Create<ProgressBarVisual>();
    _exposureBar->color = {1, 1, 1, 1};
    _exposureBar->foregroundColor = {0.4, 0.2, 0.7, 1.0f};
    _exposureBar->backgroundColor = {0.4 / 3.0, 0.2 / 3.0, 0.7 / 3.0, 0.25f};
    // _exposureBar->renderingMaterial->MakeAdditive();
    _exposureBar->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _exposureBar->renderingMaterial->renderQueue = Material::RenderQueue::RENDER_QUEUE_OVERLAY;
    _exposureBar->progress = 0.8f;
    _exposureBar->invertY = false;
    _exposureBar->sprite.SetRectPixel(12, 13, 43, 13);

    _targetHealthBar = scene->Create<ProgressBarVisual>();
    _targetHealthBar->color = {1, 1, 1, 1};
    _targetHealthBar->foregroundColor = {0.8, 0.25, 0.0, 1.0f};
    _targetHealthBar->backgroundColor = {0.8 / 3.0, 0.25 / 3.0, 0.0, 0.25f};
    // _exposureBar->renderingMaterial->MakeAdditive();
    _targetHealthBar->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _targetHealthBar->renderingMaterial->renderQueue = Material::RenderQueue::RENDER_QUEUE_OVERLAY;
    _targetHealthBar->progress = 0.8f;
    _targetHealthBar->invertY = false;
    _targetHealthBar->sprite.SetRectPixel(15, 39, 33, 7);
    _targetHealthBar->SetEnabled(false);

    // _integrityBar = scene->Create<ProgressBarVisual>();
    // _integrityBar->foregroundColor = {0.04, 1.0, 0.45, 1.0f};
    // _integrityBar->backgroundColor = {0.04 / 4.0f, 1.0 / 4.0f, 0.45 / 4.0f, 0.1f};
    // _integrityBar->renderingMaterial->MakeTransparent();
    // _integrityBar->progress = 0.1;
    _dustParticles = scene->Create<StarDustParticles>();

    _crosshair = scene->Create<SpriteVisual>();
    _crosshair->sprite.atlasTexture = atlasUITexture;
    _crosshair->sprite.SetRectPixel(8, 0, 4, 4);
    _crosshair->isScreenSpace = false;
    _crosshair->color = {1,1,1,0};
    _crosshair->position = {0.0, 0.0, 0.1};
    _crosshair->renderingMaterial->MakeInvert();

    _infoText = scene->Create<TextVisual>();
    _infoText->color = {0.8, 0.25, 0.0, 1.0f};

    _targetText = scene->Create<TextVisual>();
    _targetText->color = {0.8, 0.25, 0.0, 1.0f};
    _targetText->SetAlignment(TextVisual::Alignment::Center);

    _targetHealthText = scene->Create<TextVisual>();
    _targetHealthText->color = {0.8, 0.25, 0.0, 1.0f};
    _targetHealthText->SetAlignment(TextVisual::Alignment::Center);
    _targetHealthText->SetEnabled(false);

    _targetMesh = scene->Create<VisualMeshEntity>();
    _targetMesh->scale = {0.2, 0.2, 0.2};
    auto targetShader = CreateObject<Shader>(L"Assets/Shaders/TargetPreview.hlsl");
    targetShader->Compile();
    _targetMesh->renderingMaterial = CreateObject<Material>(targetShader);
    // _targetMesh->renderingMaterial->MakeAdditive();
    _targetMesh->renderingMaterial->renderQueue = Material::RenderQueue::RENDER_QUEUE_OVERLAY - 10;
    _targetMesh->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);

    _weapon = scene->Create<PulseWeaponEntity>(scene);
    _weapon->SetPivotOffsets({-2, -1, 0}, {2, -1, 0});
}



ShipController::~ShipController()
{
    NSE::ObjectServer::Get()->Destroy(mesh);
}

void ShipController::OnCreated()
{
    auto scene = GetScene();

    _shipRadar = scene->Create<ShipRadarController>(dynamic_pointer_cast<ShipController>(Self()));
    _shipRadar->scale = {0.6f, 0.6f, 0.6f};

    _markers = scene->Create<ShipMarkersVisual>(dynamic_pointer_cast<ShipController>(Self()));
}

void ShipController::OnUpdate()
{
    std::stringstream str;

    UpdateInfoText();

    auto input = NSE::InputServer::Get();
    auto time = NSE::TimeServer::Get();

    NSE::Vector3d targetVelocity{};
    Vector3d cameraOffset{};

    if (input->GetKey(DIK_W))
    {
        if (input->GetKey(DIK_LSHIFT))
        {
            targetVelocity += Forward() * 200.0f;
            // cameraOffset.x = (sinf(time->Time() * 20.0f) + sinf(time->Time() * 1.8415f * 20.0f) * 0.2345f) * 0.005f ;
            // cameraOffset.y = (sinf(time->Time() * 20.0f + 57.314f) + sinf(time->Time() * 1.8415f * 20.0f + 57.314f) * 0.2345f) * 0.005f;
        }
        else
        {
            targetVelocity += Forward() * 50.0f;
            // cameraOffset.x = (sinf(time->Time() * 10.0f) + sinf(time->Time() * 1.8415f * 10.0f) * 0.2345f) * 0.002f ;
            // cameraOffset.y = (sinf(time->Time() * 10.0f + 57.314f) + sinf(time->Time() * 1.8415f * 10.0f + 57.314f) * 0.2345f) * 0.002f;
        }
    }
    if (input->GetKey(DIK_S))
    {
        if (input->GetKey(DIK_LSHIFT))
            targetVelocity += Forward() * -100.0f;
        else
            targetVelocity += Forward() * -30.0f;
    }
    if (input->GetKey(DIK_D))
    {
        if (input->GetKey(DIK_LSHIFT))
            targetVelocity += Right() * 100.0f;
        else
            targetVelocity += Right() * 30.0f;
    }
    if (input->GetKey(DIK_A))
    {
        if (input->GetKey(DIK_LSHIFT))
            targetVelocity += Right() * -100.0f;
        else
            targetVelocity += Right() * -30.0f;
    }
    if (input->GetKey(DIK_SPACE))
    {
        if (input->GetKey(DIK_LSHIFT))
            targetVelocity += Up() * 100.0f;
        else
            targetVelocity += Up() * 30.0f;
    }
    if (input->GetKey(DIK_LCONTROL))
    {
        if (input->GetKey(DIK_LSHIFT))
            targetVelocity += Up() * -100.0f;
        else
            targetVelocity += Up() * -30.0f;
    }
    if (input->GetKey(DIK_R))
    {
        // Make an artificial lag spike
        float val = 45714298.94127f;
        for (int i = 0; i < 10000000; i++)
        {
            val = sqrtf(val);
        }
    }
    if (input->GetKeyDown(DIK_X))
    {
        _isShipDragEnabled = !_isShipDragEnabled;
        if (_isShipDragEnabled)
            SetInfoText("stabilizer on", 3.0f);
        else
            SetInfoText("stabilizer off", 3.0f);
    }

    if (length(targetVelocity) > 0.1f || _isShipDragEnabled)
        _shipVelocity = NSE::Vector3d::Lerp(_shipVelocity, targetVelocity, time->Delta());

    if (input->GetKey(DIK_E))
    {
        _camMomentumR = _camMomentumR + (-4.0f - _camMomentumR) * time->Delta() * 4.0f;
    }
    if (input->GetKey(DIK_Q))
    {
        _camMomentumR = _camMomentumR + (4.0f - _camMomentumR) * time->Delta() * 4.0f;
    }

    if (input->GetKeyDown(DIK_T))
    {
        auto newTarget = NavigationSystem::Get()->FindAlignedNavigatable(position, Forward(), 0.995);
        if (newTarget && _navTarget == newTarget)
        {
            _navTarget = nullptr;
        }
        else
        {
            _navTarget = newTarget;
        }
        if (_navTarget)
        {
            _targetText->SetText(_navTarget->GetNavigatableName());
            _targetMesh->mesh = _navTarget->GetNavigatablePreviewMesh();
            if (_navTarget->GetNavigatableHealth() >= 0)
            {
                _targetHealthBar->SetEnabled(true);
                _targetHealthText->SetEnabled(true);
            }
        }
        else
        {
            _targetText->SetText("");
            _targetMesh->mesh = nullptr;
            _targetHealthBar->SetEnabled(false);
            _targetHealthText->SetEnabled(false);
        }
    }

    if (_navTarget)
    {
        _targetMesh->rotation = _navTarget->GetNavigatableRotation();
        auto targetHealth = _navTarget->GetNavigatableHealth();
        str.clear();
        str << std::fixed << std::setprecision(1);
        str << targetHealth << "%";
        _targetHealthText->SetText(str.str());
        _targetHealthBar->progress = targetHealth / 100.0f;
    }

    if (_isShipDragEnabled)
        _shipVelocity *= std::clamp(1.0 - time->Delta() * 0.5, 0.0, 1.0);

    _camMomentumR *= (float)std::clamp(1.0 - time->Delta() * 4.0, 0.0, 1.0);

    int dx, dy;
    input->GetMouseDelta(dy, dx);

    _camMomentumX += (float)dx / 500.0f;
    _camMomentumY += (float)dy / 500.0f;

    _camMomentumX *= powf(0.0001f, time->Delta());
    _camMomentumY *= powf(0.0001f, time->Delta());

    double masslockedDistance;
    auto masslocked = GetClosestMasslocked(masslockedDistance);

    if (_isShiftSpaceActive)
    {
        float minVelocity = 300.0;
        float maxVelocity = 30000.0;
        float maxDistance = 100000.0;

        float shiftVelocity = maxVelocity;
        float mTime = 1.0f;
        if (masslockedDistance < maxDistance)
        {
            mTime = (masslockedDistance / maxDistance);
            mTime = std::clamp(mTime, 0.0001f, 1.0f);
            mTime = powf(mTime, 0.5);
            shiftVelocity = minVelocity + (maxVelocity - minVelocity) * mTime;
        }

        _shipVelocity = Forward() * shiftVelocity;
        _fuel -= time->Delta();
        _camMomentumX = 0.0f;
        _camMomentumY = 0.0f;

        // cameraOffset.x = (sinf(time->Time() * 30.0f) + sinf(time->Time() * 1.8415f * 30.0f) * 0.2345f) * 0.01f * mTime ;
        // cameraOffset.y = (sinf(time->Time() * 30.0f + 57.314f) + sinf(time->Time() * 1.8415f * 30.0f + 57.314f) * 0.2345f) * 0.01f * mTime;

        SetInfoText("shift drive\x88\x89", 0);

        _dustParticles->velocity = (float3)(normalize(_shipVelocity) * std::clamp(length(_shipVelocity) / 10.0, 0.0, 300.0));

        if (masslockedDistance <= 0.0)
        {
            _isShiftSpaceActive = false; // ToDo
            _shipVelocity = Forward() * 300.0f;
        }
    }
    else
    {
        _dustParticles->velocity = {};
    }

    position += _shipVelocity * time->Delta();

    auto rot = XMQuaternionRotationRollPitchYaw(_camMomentumX * time->Delta(), _camMomentumY * time->Delta(), _camMomentumR * time->Delta());
    rotation = XMQuaternionMultiply(rot, rotation);

    // _camera->position = Vector3d::Lerp(_camera->position, position + Forward() * 1.0f + Up() * 0.1f, saturate(time->Delta() * 10.0f));
    _camera->position = (position + cameraOffset) + Forward() * 1.0f + Up() * 0.1f;
    // ToDo noise

    _camera->rotation = XMQuaternionSlerp(_camera->rotation, rotation, saturate(30.0f * time->Delta()));

    // SetInfoText("caution \x8E\x8F", 0);

    _shipRadar->position = position + Forward() * 2.8f - Up() * 0.5f;
    _shipRadar->rotation = rotation;
    _shipRadar->GetParticleSystem()->position = _shipRadar->position;
    _shipRadar->GetParticleSystem()->rotation = _shipRadar->rotation;

    _fuelBar->position = _shipRadar->position + Forward() * 3.0f + Up() * 2.15f - Right() * 2.25f;
    _integrityBar->position = _shipRadar->position + Forward() * 3.0f + Up() * 2.8f - Right() * 2.25f;
    _exposureBar->position = _shipRadar->position + Forward() * 3.0f + Up() * 2.8f + Right() * 2.25f;
    _fuelBar->progress = _fuel / 100.0f;
    _integrityBar->progress = _integrity / 100.0f;
    _exposureBar->progress = _exposure / 100.0f;
    // _integrityBar->position = _shipRadar->position;
    // _integrityBar->rotation = _shipRadar->rotation;

    _targetHealthBar->position = _shipRadar->position - Up() * 0.2f - Right() * 0.85f;
    _targetHealthText->position = _shipRadar->position + Up() * 0.0f - Right() * 0.85f;

    _dustParticles->position = position;

    _weapon->position = position;
    _weapon->rotation = rotation;
    _weapon->SyncPosition();
    if (input->GetLMB())
    {
        _weapon->Fire();
    }


    _cockpitLight->position = _shipRadar->position;
    _infoText->position = position + Forward() * 3.5f + Up() * 0.5f;
    _targetText->position = position + Forward() * 3.5f - Up() * 0.6f + Right() * 1.15f;
    _targetMesh->position = position + Forward() * 3.5f - Up() * 1.0f + Right() * 1.15f;

    _crosshair->position = position + Forward() * 3.0f;

    float velDot = (float)Vector3d::Dot(_shipVelocity.Normalized(), Forward());

    _camera->SetFov(60.0f + velDot * 4.0f * (1.0f - 1.0f / (1 + (float)_shipVelocity.Magnitude() * 0.01f)));

    str.clear();
    str << std::fixed << std::setprecision(3);

    if (_shiftSpaceActivationTimer > 0)
    {
        // Charging
        _shiftSpaceActivationTimer -= time->Delta();
        if (_shiftSpaceActivationTimer <= 0.0f)
        {
            // Jump
            _isShiftSpaceActive = true;
            // ToDo VFX
            _shiftSpaceActivationTimer = 0.0f;
        }
        else
        {

        }

        if (_navTarget && _navTarget->GetNavigatableShiftSpaceNavigatable())
        {
            // ToDo check angular distance
            float3 dir = normalize((float3)(_navTarget->GetNavigatablePosition() - position));
            auto up = Up();

            auto targetRotation = Math::LookRotation(dir, float3{up.m128_f32[0], up.m128_f32[1], up.m128_f32[2]});
            rotation = XMQuaternionSlerp(rotation, targetRotation, saturate(time->Delta() * 4.0f));
        }

        str << _shiftSpaceActivationTimer;
        SetInfoText(str.str(), 0);
        _infoText->color.w = 1.0f - powf(_shiftSpaceActivationTimer / _shiftSpaceActivationTimeout, 2.0f);
    } else
    {
        // _infoText->SetText("");
    }

    if (input->GetKeyDown(DIK_G))
    {
        if (_isShiftSpaceActive)
        {
            _isShiftSpaceActive = false;
            _shipVelocity = Forward() * 1.0f;
            // _navTarget = nullptr;
            // ToDo VFX
        }
        else
        {
            if (_shiftSpaceActivationTimer > 0.0f) // If charging
            {
                // Cancel
                _shiftSpaceActivationTimer = 0.0f;
                // _navTarget = nullptr;
            }
            else // If not charging
            {
                if (masslockedDistance > 0.0)
                {
                    _shiftSpaceActivationTimer = _shiftSpaceActivationTimeout;
                    // _navTarget = NavigationSystem::Get()->FindAlignedNavigatable(position, Forward(), 0.999);
                }
                else
                {
                    SetInfoText("masslocked\x8C\x8D", 3);
                }
            }
        }
    }


}

void ShipController::UpdateInfoText()
{
    if (_textTimeout > 0.0f)
    {
        _textTimeout -= TimeServer::Get()->Delta();
    }

    if (_textTimeout <= 0.0f)
    {
        _textTimeout = 0.0f;
        _infoText->SetText("");
    }
}

obj_ptr<INavigatable> ShipController::GetClosestMasslocked(double& outSurfaceDistance)
{
    auto nav = NavigationSystem::Get();

    double minDistance = DBL_MAX;
    obj_ptr<INavigatable> result = nullptr;

    for (const auto& target : *nav)
    {
        auto distance = length(target->GetNavigatablePosition() - position);
        distance -= target->GetNavigatableMasslockRadius();

        if (distance < minDistance)
        {
            minDistance = distance;
            result = target;
        }
    }

    outSurfaceDistance = minDistance;
    return result;
}

void ShipController::SetInfoText(const std::string &text, float textTimeout)
{
    _infoText->SetText(text);
    _textTimeout = textTimeout;
}
