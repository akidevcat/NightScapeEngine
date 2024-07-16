#include "InventorySystem.h"

#include "MainSystem.h"
#include "../../engine/servers/AssetServer.h"
#include "../../engine/servers/InputServer.h"

using namespace NSE;

void InventorySystem::OnStart()
{
    auto main = MainSystem::Get();
    auto scene = main->GetMainScene();

    _background = scene->Create<SpriteVisual>();
    _background->sprite.atlasTexture = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Menu_Atlas.dds");
    _background->sprite.SetRectRectPixel(0, 0, 120, 90);
    _background->isPixelPerfect = true;
    _background->isScreenSpace = true;
    _background->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _background->renderingMaterial->renderQueue = Material::RENDER_QUEUE_OVERLAY + 1000;
    _background->SetEnabled(false);

    _tabNameText = scene->Create<TextVisual>();
    _tabNameText->SetEnabled(false);
    _tabNameText->SetText("SHIP");
    _tabNameText->position = {0, 0.0, 0};
    _tabNameText->isScreenSpace = true;
    _tabNameText->renderingMaterial->renderQueue = Material::RENDER_QUEUE_OVERLAY + 1001;
    _tabNameText->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
}

void InventorySystem::OnUpdate()
{
    auto input = InputServer::Get();

    if (input->GetKeyDown(DIK_TAB))
    {
        if (_isVisible)
            Hide();
        else
            Show();
    }
}

void InventorySystem::Show()
{
    _background->SetEnabled(true);
    _tabNameText->SetEnabled(true);
    // _nextTabNameText->SetEnabled(true);
    // _prevTabNameText->SetEnabled(true);

    _isVisible = true;
}

void InventorySystem::Hide()
{
    _background->SetEnabled(false);
    _tabNameText->SetEnabled(false);
    // _nextTabNameText->SetEnabled(false);
    // _prevTabNameText->SetEnabled(false);

    _isVisible = false;
}
