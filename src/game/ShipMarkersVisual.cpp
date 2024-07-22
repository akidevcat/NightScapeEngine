#include "ShipMarkersVisual.h"

#include "../engine/servers/AssetServer.h"
#include "../engine/servers/TimeServer.h"
#include "data/INavigatable.h"
#include "systems/MainSystem.h"
#include "systems/NavigationSystem.h"

using namespace NSE;

ShipMarkersVisual::ShipMarkersVisual()
{
    sprite.atlasTexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Atlas.dds");
    sprite.SetRectPixel(0, 8, 4, 4);
    isScreenSpace = false;
}

void ShipMarkersVisual::RenderEntity(const obj_ptr<NSE::Camera> &camera)
{
    auto nav = NavigationSystem::Get();

    for (const auto& marker : *nav)
    {
        // ToDo check shift space status
        // if ( marker->GetNavigatableShiftSpaceVisibility())

        position = camera->position + normalize(marker->GetNavigatablePosition() - camera->position);
        // todo check if target
        color = marker->GetNavigatableColor();

        SpriteVisual::RenderEntity(camera);
    }
}
