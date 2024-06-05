#include "StarsCamera.h"

#include "../engine/servers/SceneServer.h"

StarsCamera::StarsCamera(const NSE_Camera& parentCamera, const NSE_RenderTexture& targetRT)
{
    static size_t sid_PixelSizeX = NSE::ShaderUtils::PropertyToID("_PixelSizeX");
    static size_t sid_PixelSizeY = NSE::ShaderUtils::PropertyToID("_PixelSizeY");

    priority = -100;
    _parentCamera = parentCamera;
    NSE::SceneServer::Get()->CreateScene(_starsScene);
    this->targetSene = _starsScene;
    this->targetRT = targetRT;
    this->clearMode = NSE::CAMERA_CLEAR_MODE_COLOR;
    this->clearColor = {0, 0, 0, 1};

    this->CopyParams(parentCamera);

    _quadShader = NSE::CreateObject<NSE::Shader>(L"Assets/Shaders/StarQuad.hlsl");
    _quadShader->Compile();
    _quadMaterial = NSE::CreateObject<NSE::Material>(_quadShader);
    _quadMaterial->SetUnsignedInt(sid_PixelSizeX, 5);
    _quadMaterial->SetUnsignedInt(sid_PixelSizeY, 9);

    for (int i = 0; i < 100; i++)
    {
        auto quad = _starsScene->Create<NSE::QuadVisual>();

        auto rot = DirectX::XMQuaternionRotationRollPitchYaw(NSE::Math::Random() * 2.0f * DirectX::XM_PI, NSE::Math::Random() * 2.0f * DirectX::XM_PI, 0);
        quad->position = DirectX::XMMatrixRotationQuaternion(rot).r[2];
        quad->rotation = rot;
        // quad->position = DirectX::XMVector3Rotate({-1, 0, 0}, rot);
        quad->position *= 5.0f + 20.0f * NSE::Math::Random();
        quad->scale = {1, 1, 1};

        // quad->position = {0, 0, 0.5};
        quad->renderingMaterial = _quadMaterial;
        _largeStars.emplace_back(quad);
    }
}

StarsCamera::~StarsCamera()
{
    // ToDo
    // NSE::SceneServer::Get()->UnloadScene(_starsScene->GetUID());

    for (const auto& quad : _largeStars)
    {
        DestroyObject(quad);
    }

    NSE::DestroyObject(_quadMaterial);
    NSE::DestroyObject(_quadShader);
}

void StarsCamera::OnUpdate()
{
    Camera::OnUpdate();

    rotation = _parentCamera->rotation;
}
