#pragma once

#include <string>

#include "../engine/entity/Object.h"
#include "../engine/entity/SpriteVisual.h"
#include "../engine/entity/TextVisual.h"

class ItemVisualUI : public NSE::Object
{
public:
    explicit ItemVisualUI(NSE::Scene* scene, const obj_ptr<NSE::Texture2D> &atlasTexture, short queue);

    void SetPositionUI(int posX, int posY, int resX, int resY);
    void SetData(const std::string& name, int quantity);
    void SetSelected(bool state);
    void SetColor(float4 color);

    void Enable();
    void Disable();

public:
    std::string label;
    int count = 0;
    bool isSelected = false;

private:
    bool _enabled = true;
    bool _isSelected = false;
    int _posX, _posY, _resX, _resY;

    obj_ptr<NSE::SpriteVisual> _backgroundSprite = nullptr;
    obj_ptr<NSE::SpriteVisual> _pointerSprite = nullptr;
    obj_ptr<NSE::TextVisual> _labelText = nullptr;
    obj_ptr<NSE::TextVisual> _countText = nullptr;
};
