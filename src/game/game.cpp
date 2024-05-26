#include "game.h"

#include "../engine/entity/Camera.h"
#include "../engine/entity/TriangleVisual.h"

Game::Game()
{

}

Game::~Game()
{
    delete _testShader;
    delete _testMaterial;
    delete _quad;
    delete _scene;
}

bool Game::Initialize(Engine* engine)
{
    _engine = engine;

    return true;
}

void Game::Shutdown()
{

}

void Game::Start()
{
    static size_t matPropsId = ShaderUtils::PropertyToID("MaterialProperties");
    static size_t tintId = ShaderUtils::PropertyToID("Tint");

    _engine->GetSceneServer()->CreateScene(_scene);

    SceneEntity* camera = new Camera{};

    _scene->RegisterEntity(camera);

    _testShader = new Shader{L"Assets/Shaders/Triangle.hlsl"};
    _testShader->Compile(_engine->GetRenderServer()->GetDevice());

    float value = 516.3125612f;

    _testMaterial = new Material{_testShader};
    _testMaterial->SetPixelVar(tintId, &value, sizeof(float));

    // _triangle = new TriangleVisual{_engine->GetRenderServer()->GetDevice()};
    // _triangle->renderingMaterial = _testMaterial;
    _quad = new FullscreenQuad{_engine->GetRenderServer()->GetDevice()};
    _quad->renderingMaterial = _testMaterial;

    _scene->RegisterEntity(_quad);
}

bool Game::UpdateFrame()
{
    return true;
}

void Game::OnFrameInput()
{

}

void Game::OnFrameUpdate()
{

}

void Game::OnFrameRender()
{

}

void Game::OnFrameCleanup()
{

}

bool Game::Render()
{
    return true;
}
