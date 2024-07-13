#include "ShipSystem.h"

#include "MainSystem.h"

ShipSystem::ShipSystem()
{
    auto main = MainSystem::Get();
    auto mainScene = main->GetMainScene();

    _shipController = mainScene->Create<ShipController>(mainScene, main->GetEngine()->GetScreenAspect());

    _shipController->GetCamera()->colorTarget = main->GetMainColorRenderTarget();
    _shipController->GetCamera()->depthTarget = main->GetMainDepthRenderTarget();

    // _shipController->GetRadar()->AddTarget(_triangle, {1,1,1.0,1});
}
