#ifndef GAME_H
#define GAME_H

#include "../engine/engine.h"
#include "../engine/entity/FullscreenQuad.h"
#include "../engine/entity/TriangleVisual.h"
#include "../engine/render/Shader.h"
#include "../engine/render/Material.h"

class Game : public IGame
{
public:
    Game();
    ~Game();

    bool Initialize(Engine* engine);
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
    Engine* _engine = nullptr;
    Scene* _scene = nullptr;
    Shader* _testShader = nullptr;
    Material* _testMaterial = nullptr;
    TriangleVisual* _triangle = nullptr;
    // Shader* _test = nullptr;
};

#endif //GAME_H
