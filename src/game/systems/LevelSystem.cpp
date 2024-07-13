#include "LevelSystem.h"

#include "../data/PlanetCreationParameters.h"
#include "MainSystem.h"
#include "ShipSystem.h"
#include "../data/GalaxyLayer.h"

void LevelSystem::OnStart()
{
    CreateSystem(0, GalaxyLayer::Layer0);
}

void LevelSystem::CreateSystem(int seed, GalaxyLayer layer)
{
    assert(!_isSystemCreated);

    auto main = MainSystem::Get();
    auto ship = ShipSystem::Get()->GetShipController();

    int planetCount = (int)(NSE::Math::Random() * 3) + 1;

    for (int i = 0; i < planetCount; i++)
    {
        PlanetCreationParameters params;
        params.radius = 10000.0f;
        params.atmosphereHeight = 500.0f;
        params.terrainMaxHeight = 500.0f;

        auto planet = Planet::Create(params, main->GetMainScene(), main->GetScaledScene(), ship);
        planet->position = NSE::Vector3d{0, 0, 20000};

        _planetObjects.emplace_back(planet);
        // ToDo
    }

    _isSystemCreated = true;
}

void LevelSystem::DestroySystem()
{
    assert(_isSystemCreated);

    _isSystemCreated = false;
}
