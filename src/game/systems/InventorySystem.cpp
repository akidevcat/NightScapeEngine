#include "InventorySystem.h"

#include <algorithm>
#include <sstream>

#include "MainSystem.h"
#include "../../engine/servers/AssetServer.h"
#include "../../engine/servers/InputServer.h"

using namespace NSE;

void InventorySystem::OnStart()
{
    auto main = MainSystem::Get();
    auto scene = main->GetMainScene();

    _itemRegistry.emplace(0, InventoryItemDescription{0, "VOID OBJECT", "SOME VOID\nOBJECT...\n\nVERY VERY\nINTERESTING"});
    _itemRegistry.emplace(1, InventoryItemDescription{1, "EMBERITE", "USEFUL FOR\nSHIP FUEL\nREFILL"});
    _itemRegistry.emplace(2, InventoryItemDescription{2, "SPECTRITE", "USEFUL FOR\nSHIP HULL\nREPAIR"});

    _items.emplace_back(InventoryItemStack{1, 37});
    _items.emplace_back(InventoryItemStack{2, 24});
    _items.emplace_back(InventoryItemStack{0, 1});

    auto tex = AssetsServer::Get()->LoadTextureAsset(L"Assets/Textures/UI_Menu_Atlas.dds");
    auto queue = (short)(Material::RENDER_QUEUE_OVERLAY + 1001);

    _background = scene->Create<SpriteVisual>();
    _background->sprite.atlasTexture = tex;
    _background->sprite.SetRectPixel(0, 0, 120, 90);
    _background->SetPositionUI(0, 0, 120, 90);
    _background->SetAlignment(SpriteVisual::Alignment::TopLeft);
    _background->isScreenSpace = true;
    _background->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _background->renderingMaterial->renderQueue = Material::RENDER_QUEUE_OVERLAY + 1000;
    _background->SetEnabled(false);

    _tabNameSprite = scene->Create<SpriteVisual>();
    _tabNameSprite->SetEnabled(false);
    _tabNameSprite->sprite.atlasTexture = tex;
    _tabNameSprite->sprite.SetRectPixel(64, 104, 15, 5);
    _tabNameSprite->SetAlignment(SpriteVisual::Alignment::TopCenter);
    _tabNameSprite->isScreenSpace = true;
    _tabNameSprite->renderingMaterial->renderQueue = queue;
    _tabNameSprite->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _tabNameSprite->SetPositionUI(60, 2, 120, 90);

    _nextTabNameSprite = scene->Create<SpriteVisual>();
    _nextTabNameSprite->SetEnabled(false);
    _nextTabNameSprite->sprite.atlasTexture = tex;
    _nextTabNameSprite->sprite.SetRectPixel(80, 104, 23, 5);
    _nextTabNameSprite->SetAlignment(SpriteVisual::Alignment::TopCenter);
    _nextTabNameSprite->isScreenSpace = true;
    _nextTabNameSprite->renderingMaterial->renderQueue = queue;
    _nextTabNameSprite->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _nextTabNameSprite->SetPositionUI(99, 2, 120, 90);
    _nextTabNameSprite->color = {0.3, 0.3, 0.3, 1.0};

    _prevTabNameSprite = scene->Create<SpriteVisual>();
    _prevTabNameSprite->SetEnabled(false);
    _prevTabNameSprite->sprite.atlasTexture = tex;
    _prevTabNameSprite->sprite.SetRectPixel(64, 111, 27, 5);
    _prevTabNameSprite->SetAlignment(SpriteVisual::Alignment::TopCenter);
    _prevTabNameSprite->isScreenSpace = true;
    _prevTabNameSprite->renderingMaterial->renderQueue = queue;
    _prevTabNameSprite->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _prevTabNameSprite->SetPositionUI(21, 2, 120, 90);
    _prevTabNameSprite->color = {0.3, 0.3, 0.3, 1.0};

    _dSeparatorSprite = scene->Create<SpriteVisual>();
    _dSeparatorSprite->SetEnabled(false);
    _dSeparatorSprite->sprite.atlasTexture = tex;
    _dSeparatorSprite->sprite.SetRectPixel(0, 0, 1, 80);
    _dSeparatorSprite->SetAlignment(SpriteVisual::Alignment::TopLeft);
    _dSeparatorSprite->SetPositionUI(68, 9, 120, 90);
    _dSeparatorSprite->isScreenSpace = true;
    _dSeparatorSprite->renderingMaterial->renderQueue = queue;
    _dSeparatorSprite->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);

    _descriptionButtonTips = scene->Create<SpriteVisual>();
    _descriptionButtonTips->SetEnabled(false);
    _descriptionButtonTips->sprite.atlasTexture = tex;
    _descriptionButtonTips->sprite.SetRectPixel(16, 104, 45, 12);
    _descriptionButtonTips->SetAlignment(SpriteVisual::Alignment::BottomRight);
    _descriptionButtonTips->isScreenSpace = true;
    _descriptionButtonTips->renderingMaterial->renderQueue = queue;
    _descriptionButtonTips->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _descriptionButtonTips->SetPositionUI(116, 86, 120, 90);

    for (int i = 0; i < 5; i++)
    {
        auto text = scene->Create<TextVisual>();

        text->SetEnabled(false);
        text->SetAlignment(TextVisual::Alignment::TopLeft);
        text->isScreenSpace = true;
        text->renderingMaterial->renderQueue = queue + 1;
        text->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
        text->SetPositionUI(71, 11 + i * 7, 120, 90);
        text->color = {1.0f, powf(157.0f / 255.0f, 2.2f), powf(60.0f / 255.0f, 2.2f), 1.0f};

        _descriptionTexts.emplace_back(text);
    }

    for (int i = 0; i < 10; i++)
    {
        auto item = CreateObject<ItemVisualUI>(scene, tex, queue);

        item->Disable();
        item->SetPositionUI(2, 11 + i * 9, 120, 90);
        item->SetData("EMBERITE", 37);

        _itemVisuals.emplace_back(item);
    }
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

    if (!_isVisible)
    {
        return;
    }

    if (input->GetKeyDown(DIK_DOWNARROW) || input->GetKeyDown(DIK_S))
    {
        _selectedItemIndex++;

        _selectedItemIndex = std::clamp(_selectedItemIndex, -1, (int)_items.size() - 1);

        RefreshCargoUI();
        RefreshCargoDescriptionUI();
    }

    if (input->GetKeyDown(DIK_UPARROW) || input->GetKeyDown(DIK_W))
    {
        _selectedItemIndex--;

        _selectedItemIndex = std::clamp(_selectedItemIndex, -1, (int)_items.size() - 1);

        RefreshCargoUI();
        RefreshCargoDescriptionUI();
    }
}

void InventorySystem::Show()
{
    _background->SetEnabled(true);
    _tabNameSprite->SetEnabled(true);
    _nextTabNameSprite->SetEnabled(true);
    _prevTabNameSprite->SetEnabled(true);
    _dSeparatorSprite->SetEnabled(true);
    _descriptionButtonTips->SetEnabled(true);

    for (const auto& text : _descriptionTexts)
    {
        text->SetEnabled(true);
    }

    for (const auto& item : _itemVisuals)
    {
        item->Enable();
    }

    _isVisible = true;

    _selectedItemIndex = std::clamp(_selectedItemIndex, -1, (int)_items.size() - 1);

    RefreshCargoUI();
    RefreshCargoDescriptionUI();
}

void InventorySystem::Hide()
{
    _background->SetEnabled(false);
    _tabNameSprite->SetEnabled(false);
    _nextTabNameSprite->SetEnabled(false);
    _prevTabNameSprite->SetEnabled(false);
    _dSeparatorSprite->SetEnabled(false);
    _descriptionButtonTips->SetEnabled(false);

    for (const auto& text : _descriptionTexts)
    {
        text->SetEnabled(false);
    }

    for (const auto& item : _itemVisuals)
    {
        item->Disable();
    }

    _isVisible = false;
}

void InventorySystem::RefreshCargoDescriptionUI()
{
    if (_selectedItemIndex < 0 || _selectedItemIndex >= _items.size())
    {
        for (const auto& text : _descriptionTexts)
        {
            text->SetText("");
        }
        return;
    }

    const auto& item = _items[_selectedItemIndex];
    const auto& description = _itemRegistry.at(item.Id);

    std::stringstream ss{description.Description};
    std::string line;
    int lineIndex = 0;

    while (std::getline(ss, line) && lineIndex < 5)
    {
        _descriptionTexts[lineIndex]->SetText(line);

        lineIndex++;
    }

    for (; lineIndex < 5; lineIndex++)
    {
        _descriptionTexts[lineIndex]->SetText("");
    }
}

void InventorySystem::RefreshCargoUI()
{
    for (int i = 0; i < _itemVisuals.size(); i++)
    {
        if (i >= _items.size())
        {
            _itemVisuals[i]->Disable();
            continue;
        }

        const auto& item = _items[i];
        const auto& itemDescription = _itemRegistry.at(item.Id);

        _itemVisuals[i]->Enable();
        _itemVisuals[i]->SetSelected(i == _selectedItemIndex);
        _itemVisuals[i]->SetData(itemDescription.Name, item.Quantity);
    }
}
