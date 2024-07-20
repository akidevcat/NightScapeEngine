#include "LevelSystem.h"

#include <iostream>
#include <ostream>

#include "../data/PlanetCreationParameters.h"
#include "MainSystem.h"
#include "ShipSystem.h"
#include "../data/GalaxyLayer.h"

using namespace NSE;

void LevelSystem::OnStart()
{
    CreateSystem((int)TimeServer::Get()->NowReal(), GalaxyLayer::Layer0);
}

void LevelSystem::CreateSystem(int seed, GalaxyLayer layer)
{
    assert(!_isSystemCreated);

    auto main = MainSystem::Get();
    auto ship = ShipSystem::Get()->GetShipController();

    srand(seed);

    int planetCount = (int)(NSE::Math::Random() * 6.0f) + 1;
    float maxPlanetRadius = 40000.0f;
    float minPlanetRadius = 20000.0f;
    float maxVerticalOffset = 20000.0f;
    float slotFreeAreaMultiplier = 1.0f;

    int planetSlotsStride = 3;
    std::vector<std::pair<int, int>> planetSlots;

    for (int y = -planetSlotsStride / 2, i = 0; y <= planetSlotsStride / 2; y++)
    {
        for (int x = -planetSlotsStride / 2; x <= planetSlotsStride / 2; x++, i++)
        {
            if (x == 0 && y == 0)
                continue;

            planetSlots.emplace_back(x, y);
        }
    }

    for (int i = 0; i < planetCount; i++)
    {
        PlanetCreationParameters params;
        params.radius = minPlanetRadius + (maxPlanetRadius - minPlanetRadius) * NSE::Math::Random();
        params.atmosphereHeight = params.radius / 20.0f;
        params.terrainMaxHeight = params.radius / 20.0f;

        if (Math::Random() > 0.5f)
            params.noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
        else
            params.noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);

        params.noise.SetSeed(seed + i);
        params.noise.SetFrequency(2.0f + Math::Random() * 8.0f);
        params.noise.SetFractalLacunarity(2.0f + Math::Random() * 2.0f);
        params.noise.SetFractalGain(0.15f + Math::Random() * 0.25f);
        params.noise.SetFractalOctaves(3);

        if (Math::Random() > 0.5f)
            params.noise.SetFractalType(FastNoiseLite::FractalType_FBm);
        else
            params.noise.SetFractalType(FastNoiseLite::FractalType_Ridged);

        if (Math::Random() > 0.5f)
        {
            params.noise.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2Reduced);
            params.noise.SetDomainWarpAmp(2.0f + Math::Random() * 6.0f);
        }
        else
        {

        }

        params.primaryColor = float4{Math::Random(), Math::Random(), Math::Random(), 1.0};

        auto planet = Planet::Create(params, main->GetMainScene(), main->GetScaledScene(), ship);

        int planetSlot = (int)(NSE::Math::Random() * (float)planetSlots.size());
        auto slotPos = planetSlots[planetSlot];
        float posX = (float)slotPos.first * (2.0f * maxPlanetRadius * slotFreeAreaMultiplier);
        float posY = (float)slotPos.second * (2.0f * maxPlanetRadius * slotFreeAreaMultiplier);

        float maxMovement = maxPlanetRadius * slotFreeAreaMultiplier - params.radius;
        posX += maxMovement * (NSE::Math::Random() * 2.0f - 1.0f);
        posY += maxMovement * (NSE::Math::Random() * 2.0f - 1.0f);
        float posZ = maxVerticalOffset * (NSE::Math::Random() * 2.0f - 1.0f);

        planet->position = NSE::Vector3d{posX, posZ, posY};

        planetSlots.erase(planetSlots.begin() + planetSlot);
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
