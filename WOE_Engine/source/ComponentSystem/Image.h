#pragma once

#include "Component.h"
#include "EntitySystem/Entity.h"
#include "SDL/SDL.h"
#include <iostream>

class Image :
    public Component
{
protected:
    StringId m_type = "Image";
public:
    Entity* m_pOwner;
    SDL_Texture* m_pTexture;
    SDL_FRect m_transform;
    bool m_activeRendering;

    Image(Entity* pOwner, const char* path, SDL_Renderer* pRenderer, float width, float height);
    ~Image();

    void Render([[maybe_unused]] SDL_Renderer* pRenderer, Vec2 cameraOffset) override;
    void TurnOffRendering() { m_activeRendering = false; std::cout << "Image Rendering turned Off\n"; }
    StringId GetType() override { return m_type; }
};
