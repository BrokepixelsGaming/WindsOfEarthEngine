#pragma once
#include "Component.h"
#include "EngineInterface/EngineSingleton.h"

#include <SDL/SDL_ttf.h>


///////////////////////////////////////////////////////////////
// Text Field implementation
///////////////////////////////////////////////////////////////
class UITextField :
    public Component
{
private:
    // Position and dimentsions.
    SDL_FRect m_transform;

    // Image
    SDL_Texture* m_pTexture;
    StringId m_type = "UITextField";
    TTF_Font* m_pFont;
    SDL_Color m_color;

public:
    UITextField(TTF_Font* pFont, const char* pText, SDL_Color color, SDL_FRect position, SDL_Renderer* pRenderer);
    ~UITextField();

    StringId GetType() override { return m_type; }
    virtual void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset) override;
    void UpdateLocation(float x, float y);
    void SetHeightWidth(float h, float w);
    void UpdateText(const char* pText);


    // Inherited via GameObject
    //virtual const std::string GetName() override;

};