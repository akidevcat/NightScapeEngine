#ifndef GAME_H
#define GAME_H

#include "../engine/engine.h"
#include "../engine/entity/QuadVisual.h"
#include "../engine/entity/TriangleVisual.h"
#include "../engine/render/Shader.h"
#include "../engine/render/Material.h"
#include "systems/GalaxySystem.h"
#include "systems/GenerationSystem.h"
#include "systems/InventorySystem.h"
#include "systems/LevelSystem.h"
#include "systems/MainSystem.h"
#include "systems/NavigationSystem.h"
#include "systems/ShipSystem.h"

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
    void OnBeginFrameUpdate() override;
    void OnFinishFrameUpdate() override;
    void OnFrameRender() override;
    void OnFrameCleanup() override;

private:
    bool Render();

private:
    MainSystem* _mainSystem = nullptr;
    GalaxySystem* _galaxySystem = nullptr;
    GenerationSystem* _generationSystem = nullptr;
    InventorySystem* _inventorySystem = nullptr;
    LevelSystem* _levelSystem = nullptr;
    NavigationSystem* _navigationSystem = nullptr;
    ShipSystem* _shipSystem = nullptr;

    NSE::Engine* _engine = nullptr;
    NSE::Scene* _scene = nullptr;
    NSE::Scene* _presentScene = nullptr;
    // NSE_Shader _testShader = nullptr;
    // NSE_Material _testMaterial = nullptr;
    // NSE_TriangleVisual _triangle = nullptr;
    // NSE_RenderTexture _cameraRT = nullptr;
    // NSE_RenderTexture _renderColorRT = nullptr;
    // NSE_RenderTexture _renderDepthRT = nullptr;
    // NSE_Camera _presentCamera = nullptr;
    // NSE_QuadVisual _presentPlane = nullptr;
    // NSE_Shader _presentShader = nullptr;
    // NSE_Material _presentMaterial = nullptr;
    // Shader* _test = nullptr;
};

#endif //GAME_H
