#include "NSE.h"
#include <catch2/catch_all.hpp>

#include "Engine/Runtime/Memory/FactoryRegistry.h"

struct RandomComponent
{
    float value;
};

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

    void OnRegisteringComponents(NSE::Ref<NSE::FactoryRegistry> registry) override
    {
        registry->RegisterComponent<RandomComponent>();
    }
};

TEST_CASE("Engine basic testing", "[User]")
{
    auto game = new TestGame{};
    auto engine = NSE::CreateEngine(game);

    NSE::EngineConfiguration cfg
    {
        "Test App",
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
