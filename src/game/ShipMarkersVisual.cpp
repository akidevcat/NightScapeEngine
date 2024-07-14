#include "ShipMarkersVisual.h"

#include "../engine/servers/AssetServer.h"
#include "data/INavigatable.h"
#include "systems/MainSystem.h"

ShipMarkersVisual::ShipMarkersVisual()
{
    sprite.atlasTexture = NSE::AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Atlas.dds");
    sprite.SetRectRectPixel(0, 0, 8, 8);
    isPixelPerfect = true;
    isScreenSpace = false;
}

void ShipMarkersVisual::RenderEntity(const obj_ptr<NSE::Camera> &camera)
{
    auto main = MainSystem::Get();
    auto scene = main->GetMainScene();

    std::vector<obj_ptr<INavigatable>> targets;
    scene->FindAllEntitiesFromBaseType<INavigatable>(targets);

    position = {};

    for (const auto& marker : targets)
    {
        auto test = marker->GetNavigatablePosition();
        // position = test;
        // color = marker->GetNavigatableColor();
        // SpriteVisual::RenderEntity(camera);
    }
}
