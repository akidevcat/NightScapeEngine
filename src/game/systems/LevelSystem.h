#pragma once

#include <vector>

#include "GameSystem.h"
#include "../../engine/obj_ptr.h"
#include "../planetary/ISpaceObject.h"
#include "../planetary/Planet.h"

enum class GalaxyLayer;

class LevelSystem : public GameSystem<LevelSystem>
{
public:
    void OnStart() override;

    void CreateSystem(int seed, GalaxyLayer layer);
    void DestroySystem();

private:
    bool _isSystemCreated = false;
    obj_ptr<ISpaceObject> _sunObject = nullptr;
    std::vector<obj_ptr<Planet>> _planetObjects;
};
