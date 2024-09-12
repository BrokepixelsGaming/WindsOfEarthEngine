#pragma once
#include "Component.h"
#include "..\3rdParty\include\SDL\SDL.h"
#include "tools/Utilities/Rect.h"
#include "tools/Utilities/Vector.h"
#include "tools/Utilities/Color.h"
#include "EntitySystem/Entity.h"

///////////////////////////////////////////////////////////////
// The intended purpose is as a place holder for an entity. Ideally used to render a collision box. 
///////////////////////////////////////////////////////////////
class RenderRect :
    public Component
{
	Color m_color;
	SDL_FRect m_sdlRect;
	Entity* m_pOwner;
protected:
	StringId m_type = "RenderRect";
public:
	RenderRect(Rect rect, Vec2 location, Color color, Entity* owner);
	void Render([[maybe_unused]] SDL_Renderer* pRenderer, Vec2 cameraOffset) override;
};
