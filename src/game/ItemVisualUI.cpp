#include "ItemVisualUI.h"

using namespace NSE;

ItemVisualUI::ItemVisualUI(Scene* scene, const obj_ptr<Texture2D> &atlasTexture, short queue)
{
    _backgroundSprite = scene->Create<SpriteVisual>();
    _backgroundSprite->sprite.atlasTexture = atlasTexture;
    _backgroundSprite->sprite.SetRectPixel(0, 90, 63, 7);
    _backgroundSprite->SetAlignment(SpriteVisual::Alignment::TopLeft);
    _backgroundSprite->isScreenSpace = true;
    _backgroundSprite->renderingMaterial->renderQueue = queue;
    _backgroundSprite->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);

    _pointerSprite = scene->Create<SpriteVisual>();
    _pointerSprite->sprite.atlasTexture = atlasTexture;
    _pointerSprite->sprite.SetRectPixel(63, 98, 3, 5);
    _pointerSprite->SetAlignment(SpriteVisual::Alignment::TopLeft);
    _pointerSprite->isScreenSpace = true;
    _pointerSprite->renderingMaterial->renderQueue = queue;
    _pointerSprite->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _pointerSprite->SetEnabled(false);

    _labelText = scene->Create<TextVisual>();
    _labelText->SetAlignment(TextVisual::Alignment::TopLeft);
    _labelText->isScreenSpace = true;
    _labelText->renderingMaterial->renderQueue = queue + 1;
    _labelText->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _labelText->SetEnabled(false);

    _countText = scene->Create<TextVisual>();
    _countText->SetAlignment(TextVisual::Alignment::TopRight);
    _countText->isScreenSpace = true;
    _countText->renderingMaterial->renderQueue = queue + 1;
    _countText->renderingMaterial->SetDepthWrite(ShaderDepthState::Disabled);
    _countText->SetEnabled(false);

    SetColor({1.0f, powf(157.0f / 255.0f, 2.2f), powf(60.0f / 255.0f, 2.2f), 1.0f}); // ToDo make a separate method
}

void ItemVisualUI::SetPositionUI(const int posX, const int posY, const int resX, const int resY)
{
    _backgroundSprite->SetPositionUI(posX + 1, posY, resX, resY); // todo if selected
    _pointerSprite->SetPositionUI(posX, posY + 1, resX, resY);
    _labelText->SetPositionUI(posX + 3 + (_isSelected ? 3 : 0), posY + 1, resX, resY);
    _countText->SetPositionUI(posX + 63, posY + 1, resX, resY);

    _posX = posX;
    _posY = posY;
    _resX = resX;
    _resY = resY;
}

void ItemVisualUI::SetData(const std::string &name, int quantity)
{
    _labelText->SetText(name);
    _countText->SetText(std::to_string(quantity));
}

void ItemVisualUI::SetSelected(bool state)
{
    if (state)
    {
        _backgroundSprite->sprite.SetRectPixel(0, 97, 63, 7);
    }
    else
    {
        _backgroundSprite->sprite.SetRectPixel(0, 90, 63, 7);

    }

    _labelText->SetPositionUI(_posX + 3 + (state ? 3 : 0), _posY + 1, _resX, _resY);

    if (_enabled && state)
    {
        _pointerSprite->SetEnabled(true);

    }
    else
    {
        _pointerSprite->SetEnabled(false);

    }

    _isSelected = state;
}

void ItemVisualUI::SetColor(float4 color)
{
    _labelText->color = color;
    _countText->color = color;
}

void ItemVisualUI::Enable()
{
    _backgroundSprite->SetEnabled(true);
    _labelText->SetEnabled(true);
    _countText->SetEnabled(true);

    if (_isSelected)
    {
        _pointerSprite->SetEnabled(true);
    }

    _enabled = true;
}

void ItemVisualUI::Disable()
{
    _backgroundSprite->SetEnabled(false);
    _pointerSprite->SetEnabled(false);
    _labelText->SetEnabled(false);
    _countText->SetEnabled(false);

    _enabled = false;
}
