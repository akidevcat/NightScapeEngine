#ifndef GAME_H
#define GAME_H

#include "../engine/engine.h"
#include "../engine/entity/QuadVisual.h"
#include "../engine/entity/TriangleVisual.h"
#include "../engine/render/Shader.h"
#include "../engine/render/Material.h"

class Game : public NSE::IGame
{
public:
    Game();
    ~Game();

    bool Initialize(NSE::Engine* engine);
    void Shutdown();
    bool UpdateFrame();

    void Start() override;
    void OnFrameInput() override;
    void OnFrameUpdate() override;
    void OnFrameRender() override;
    void OnFrameCleanup() override;

private:
    bool Render();

private:
    NSE::Engine* _engine = nullptr;
    NSE::Scene* _scene = nullptr;
    NSE::Scene* _presentScene = nullptr;
    NSE_Shader _testShader = nullptr;
    NSE_Material _testMaterial = nullptr;
    NSE_TriangleVisual _triangle = nullptr;
    NSE_RenderTexture _cameraRT = nullptr;
    NSE_Camera _presentCamera = nullptr;
    NSE_QuadVisual _presentPlane = nullptr;
    NSE_Shader _presentShader = nullptr;
    NSE_Material _presentMaterial = nullptr;
    // Shader* _test = nullptr;
};

#endif //GAME_H
