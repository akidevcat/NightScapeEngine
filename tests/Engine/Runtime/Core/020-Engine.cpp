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

TEST_CASE("Engine basic testing", "[User]")
{
    auto game = new TestGame{};
    auto engine = NSE::CreateEngine(game);

    NSE::EngineConfiguration cfg
    {
        NSE::EngineConfiguration::VSyncMode::None
    };
    engine->Initialize(cfg);

    while(true)
    {
        if (!engine->UpdateFrame())
            break;
    }

    REQUIRE(true);

    engine->Shutdown();
    free(game);
}
