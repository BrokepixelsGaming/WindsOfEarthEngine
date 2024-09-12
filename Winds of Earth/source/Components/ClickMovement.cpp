#include "ClickMovement.h"
#include "EngineInterface/EngineSingleton.h"
#include "SelectorComponent.h"

ClickMovement::ClickMovement(Entity* pOwner)
	: m_pOwner(pOwner)
	, m_flyer(false)
	, m_targetLocation(pOwner->GetLocation())
{
}

ClickMovement::ClickMovement(Entity* pOwner, bool flyer)
	: m_pOwner(pOwner)
	, m_flyer(flyer)
	, m_targetLocation(pOwner->GetLocation())
{
}

void ClickMovement::Update(double deltaTime)
{
	if (!m_path.empty())
	{
		StepMovement(deltaTime);
	}
}

bool ClickMovement::ProcessEvent(Event& evt)
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
			if (sdlEvt->button.button == '\x3') 
			{
					m_targetLocation =  Vec2{ static_cast<float>(sdlEvt->button.x + EngineSingleton::Get()->GetCamera().GetOffsetX()), static_cast<float>(sdlEvt->button.y + EngineSingleton::Get()->GetCamera().GetOffsetY()) };
					// Set new path.
					m_path.clear();
					m_path = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GeneratePathToBestTarget(m_pOwner->GetLocation(), m_targetLocation);
			}
			if (sdlEvt->button.button == '\x1')
			{

				if (EngineSingleton::Get()->GetKeyboard().IsKeyDown(kTKey))
				{
					m_pOwner->SetLocation( Vec2{ static_cast<float>(sdlEvt->button.x + EngineSingleton::Get()->GetCamera().GetOffsetX()), static_cast<float>(sdlEvt->button.y + EngineSingleton::Get()->GetCamera().GetOffsetY()) });
				}
			}
		}
		}
	}
	return false;
}

void ClickMovement::StepMovement(double deltaTimeMs)
{

	// step the game object towards their goal
	Vec2 direction = m_path.front() - GetPos();
	direction.Normalize();
	Vec2 deltaPosition = direction * kDefaultSpeed * (float)deltaTimeMs;
	m_pOwner->UpdateLocation(deltaPosition);

	// check to see if we've arrived
	Vec2 delta = m_path.front() - GetPos();
	if (delta.Length() <= kPathingNodeTolerance)
		m_path.pop_front();
}
