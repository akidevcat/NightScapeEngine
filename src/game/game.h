#ifndef GAME_H
#define GAME_H

#include "../engine/engine.h"

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
    Engine* _engine;
};

#endif //GAME_H
