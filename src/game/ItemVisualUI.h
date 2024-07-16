#pragma once

#include <string>

#include "../engine/entity/Object.h"
#include "../engine/entity/SpriteVisual.h"
#include "../engine/entity/TextVisual.h"

class ItemVisualUI : public NSE::Object
{
public:
    std::string label;
    int count = 0;
    bool isSelected = false;

private:
    obj_ptr<NSE::SpriteVisual> _backgroundSprite = nullptr;
    obj_ptr<NSE::SpriteVisual> _pointerSprite = nullptr;
    obj_ptr<NSE::TextVisual> _labelText = nullptr;
    obj_ptr<NSE::TextVisual> _countText = nullptr;
};
