#pragma once

#include "GameSystem.h"
#include "../../engine/entity/Camera.h"
#include "../../engine/entity/QuadVisual.h"

namespace NSE
{
    class Scene;
}

class MainSystem : public GameSystem<MainSystem>
{
public:
    MainSystem(NSE::Engine* engine);
    ~MainSystem();

    void OnStart() override;
    void OnUpdate() override;

    [[nodiscard]] NSE::Engine* GetEngine() const { return _engine; }
    [[nodiscard]] NSE::Scene* GetMainScene() const { return _mainScene; }
    [[nodiscard]] NSE::Scene* GetScaledScene() const { return _scaledScene; }
    [[nodiscard]] NSE::Scene* GetDisplayScene() const { return _displayScene; }
    [[nodiscard]] NSE_RenderTexture GetMainColorRenderTarget() const { return _mainColorRenderTarget; }
    [[nodiscard]] NSE_RenderTexture GetMainDepthRenderTarget() const { return _mainDepthRenderTarget; }

private:
    NSE::Engine* _engine = nullptr;

    NSE::Scene* _mainScene = nullptr;
    NSE::Scene* _scaledScene = nullptr;
    NSE::Scene* _displayScene = nullptr;

    NSE_Camera          _displayCamera = nullptr;
    NSE_Camera          _scaledCamera = nullptr;
    NSE_QuadVisual      _displayScreen = nullptr;
    NSE_Material        _displayScreenMaterial = nullptr;
    NSE_RenderTexture   _mainColorRenderTarget = nullptr;
    NSE_RenderTexture   _mainDepthRenderTarget = nullptr;

    NSE_QuadVisual      _skyboxQuad = nullptr;

};
