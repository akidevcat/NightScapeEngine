#include "NSE.h"
#include <catch2/catch_all.hpp>

class TestGame : public NSE::IAppInstance
{
public:
    TestGame()
    {
    }

    ~TestGame() override
    {

    }

    void OnGameLaunch() override
    {

    }
};

TEST_CASE("Engine basic testing", "[Engine]")
{
    auto game = new TestGame{};
    auto engine = NSE::CreateEngine(game);
    NSE::EngineConfiguration cfg
    {
        NSE::EngineConfiguration::VSyncMode::None
    };
    engine->Initialize(cfg);
    // Render some frames
    for (int i = 0; i < 10; i++)
    {
        engine->UpdateFrame();
    }
    REQUIRE(true);
    engine->Shutdown();
    free(game);
}
