#include "RenderRect.h"
#include "..\3rdParty\include\SDL\SDL.h"
#include "EngineInterface/EngineSingleton.h"

RenderRect::RenderRect(Rect rect, Vec2 location, Color color, Entity* owner)
	: m_color(color)
	, m_pOwner(owner)
{
	m_sdlRect.x = location.x;
	m_sdlRect.y = location.y;
	m_sdlRect.h = static_cast<float>(rect.height);
	m_sdlRect.w = static_cast<float>(rect.width);
}

void RenderRect::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{

	// Get the location of the owner. 
	auto location = m_pOwner->GetLocation();

	// We will update our internal rect at this point.
	m_sdlRect.x = location.x - (m_sdlRect.w / 2);
	m_sdlRect.y = location.y - (m_sdlRect.h / 2);


	SDL_FRect rect{ m_sdlRect.x - cameraOffset.x, m_sdlRect.y - cameraOffset.y, m_sdlRect.w, m_sdlRect.h };

	// Here we setting the render for the struct. 
	SDL_SetRenderDrawColor(pRenderer, m_color.red, m_color.green, m_color.blue, m_color.alpha);
	SDL_RenderFillRectF(pRenderer, &rect);

	// Here we will set the Render color back to a default color. Might move a constant to Settings.h but for now this works and is the only place. 
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255); // setting to white. temp solution. 
	
}