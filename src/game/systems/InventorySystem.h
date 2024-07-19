#pragma once

#include "GameSystem.h"
#include "../ItemVisualUI.h"
#include "../../engine/obj_ptr.h"
#include "../../engine/entity/SpriteVisual.h"
#include "../../engine/entity/TextVisual.h"
#include "../data/InventoryItemStack.h"
#include "../data/InventoryItemDescription.h"

class InventorySystem : GameSystem<InventorySystem>
{
public:
    void OnStart() override;
    void OnUpdate() override;

    void Show();
    void Hide();

private:
    void RefreshCargoDescriptionUI();
    void RefreshCargoUI();

    bool _isVisible = false;
    int _selectedItemIndex = -1;

    obj_ptr<NSE::SpriteVisual> _background = nullptr;
    obj_ptr<NSE::SpriteVisual> _tabNameSprite = nullptr;
    obj_ptr<NSE::SpriteVisual> _nextTabNameSprite = nullptr;
    obj_ptr<NSE::SpriteVisual> _prevTabNameSprite = nullptr;
    obj_ptr<NSE::SpriteVisual> _dSeparatorSprite = nullptr;
    obj_ptr<NSE::SpriteVisual> _descriptionButtonTips = nullptr;

    std::vector<obj_ptr<NSE::TextVisual>> _descriptionTexts;
    std::vector<obj_ptr<ItemVisualUI>> _itemVisuals;

    std::vector<InventoryItemStack> _items;
    std::unordered_map<char, InventoryItemDescription> _itemRegistry;
};
