#pragma once
#include "GameSystem.h"
#include "../../engine/obj_ptr.h"
#include "../../engine/entity/SpriteVisual.h"
#include "../../engine/entity/TextVisual.h"

class InventorySystem : GameSystem<InventorySystem>
{

public:
    void OnStart() override;
    void OnUpdate() override;

    void Show();
    void Hide();

private:
    bool _isVisible = false;

    obj_ptr<NSE::SpriteVisual> _background = nullptr;
    obj_ptr<NSE::TextVisual> _tabNameText = nullptr;
    obj_ptr<NSE::TextVisual> _nextTabNameText = nullptr;
    obj_ptr<NSE::TextVisual> _prevTabNameText = nullptr;

    std::vector<obj_ptr<NSE::TextVisual>> _descriptionTexts;

};
