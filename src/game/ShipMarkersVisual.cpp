#include "ShipMarkersVisual.h"

#include "ShipController.h"
#include "../engine/servers/AssetServer.h"
#include "../engine/servers/TimeServer.h"
#include "data/INavigatable.h"
#include "systems/MainSystem.h"
#include "systems/NavigationSystem.h"

using namespace NSE;

ShipMarkersVisual::ShipMarkersVisual(const obj_ptr<ShipController>& controller)
{
    _controller = controller;
    sprite.atlasTexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Atlas.dds");
    sprite.SetRectPixel(0, 8, 4, 4);
    isScreenSpace = false;
}

void ShipMarkersVisual::RenderEntity(const obj_ptr<NSE::Camera> &camera)
{
    auto nav = NavigationSystem::Get();
    auto navTarget = _controller->GetNavTarget();

    for (const auto& marker : *nav)
    {
        // ToDo check shift space status
        // if ( marker->GetNavigatableShiftSpaceVisibility())
        if (marker == navTarget)
        {
            color = {1.00, 0.547, 0.0300, 1.0};
        }
        else
        {
            color = {1,1,1,1};
        }

        position = camera->position + normalize(marker->GetNavigatablePosition() - camera->position);
        // todo check if target


        SpriteVisual::RenderEntity(camera);
    }
}
