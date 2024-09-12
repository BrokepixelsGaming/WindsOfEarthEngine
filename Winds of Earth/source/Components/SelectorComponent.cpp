#include "SelectorComponent.h"
#include "EngineInterface/EngineSingleton.h"
#include <iostream>

SelectorComponent::SelectorComponent(Entity* owner, float ownerHeight, float ownerWidth)
	: m_pOwner(owner)
	, m_ownerHeight(ownerHeight)
	, m_ownerWidth(ownerWidth)
	, m_selected(false)
{
}

SelectorComponent::~SelectorComponent()
{
}

void SelectorComponent::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	Vec2 topLeft = { ((m_pOwner->GetLocation().x - (m_ownerWidth / 2)) - kSpacing), ((m_pOwner->GetLocation().y - (m_ownerHeight / 2))- kSpacing)};
	Vec2 topRight = { ((m_pOwner->GetLocation().x + (m_ownerWidth / 2)) + kSpacing),((m_pOwner->GetLocation().y - (m_ownerHeight / 2)) - kSpacing)};
	Vec2 bottomLeft = { ((m_pOwner->GetLocation().x - (m_ownerWidth / 2)) - kSpacing),((m_pOwner->GetLocation().y + (m_ownerHeight / 2)) + kSpacing)};
	Vec2 bottomRight = { ((m_pOwner->GetLocation().x + (m_ownerWidth / 2)) + kSpacing),((m_pOwner->GetLocation().y + (m_ownerHeight / 2)) + kSpacing)};

	if (m_selected)
	{
		SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
		// top left corner to bottom left. 
		SDL_RenderDrawLine(pRenderer, static_cast<int>(topLeft.x - cameraOffset.x), static_cast<int>(topLeft.y - cameraOffset.y), static_cast<int>(bottomLeft.x - cameraOffset.x), static_cast<int>(bottomLeft.y - cameraOffset.y));
		// top Left corner to top Right
		SDL_RenderDrawLine(pRenderer, static_cast<int>(topLeft.x - cameraOffset.x), static_cast<int>(topLeft.y - cameraOffset.y), static_cast<int>(topRight.x - cameraOffset.x), static_cast<int>(topRight.y - cameraOffset.y));
		// top right corner to bottom Right
		SDL_RenderDrawLine(pRenderer, static_cast<int>(topRight.x - cameraOffset.x), static_cast<int>(topRight.y - cameraOffset.y), static_cast<int>(bottomRight.x - cameraOffset.x), static_cast<int>(bottomRight.y - cameraOffset.y));
		// bottom right corner to bottom left
		SDL_RenderDrawLine(pRenderer, static_cast<int>(bottomRight.x - cameraOffset.x), static_cast<int>(bottomRight.y - cameraOffset.y), static_cast<int>(bottomLeft.x - cameraOffset.x), static_cast<int>(bottomLeft.y -  cameraOffset.y));
	}
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
}

bool SelectorComponent::ProcessEvent(Event& evt)
{
	auto sdlEvt = evt.GetSDLEvent();
	if (sdlEvt)
	{
		switch (sdlEvt->type)
		{
		case SDL_MOUSEBUTTONUP:
		{
			//std::cout << sdlEvt->button.button;
			// '\x1' is left click '\x3' is right click
			if (sdlEvt->button.button == '\x1') {
				if (HitTest(static_cast<int>(evt.GetSDLEvent()->button.x + EngineSingleton::Get()->GetCamera().GetOffsetX()), static_cast<int>(evt.GetSDLEvent()->button.y + (EngineSingleton::Get()->GetCamera().GetOffsetY()))))
				{
					m_selected = true;
				}
				else
				{
					m_selected = false;
				}
			}
		}
		}
	}
	return false;
}

bool SelectorComponent::HitTest(int x, int y)
{
	return(x >= m_pOwner->GetLocation().x - (m_ownerWidth / 2) && x <= m_pOwner->GetLocation().x + (m_ownerWidth / 2) &&
			y >= m_pOwner->GetLocation().y - (m_ownerHeight / 2) && y <= m_pOwner->GetLocation().y + (m_ownerHeight /2));
}
