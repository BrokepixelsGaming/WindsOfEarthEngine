#include "PlayerMoveComponent.h"
#include "EventSystem/Event.h"
#include "config/Settings.h"
#include "EngineInterface/EngineSingleton.h"
//#include <BleachLua/LuaState.h>
#include "../tools/Utilities/StringId.h"
#include <iostream>
#include "tools/Utilities/Vector.h"

PlayerMoveComponent::PlayerMoveComponent(Entity* owner)
	: MoveComponent(owner)
	, m_pImage(nullptr)
{
	m_changeInMove = { 0,0 };
}

PlayerMoveComponent::~PlayerMoveComponent()
{
	m_pImage = nullptr;
	m_path.clear();
}

bool PlayerMoveComponent::ProcessEvent([[maybe_unused]] Event& evt)
{
	std::string ghost = "Ghost";
	if (evt.GetTargetEntityID() == m_pOwner->GetID())
	{
		if (evt.GetEventType() == Event::EventType::Collision)
		{
			auto& ghost1 = evt.GetCommandString();

			if (ghost1 == ghost)
			{
				if (!EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->PowerUp())
				{
					std::cout << "You hit ghost and died.\n";
					EngineSingleton::Get()->GetEventManager()->QueueEvent(Event(Event::EventType::DestroyEntity, m_pOwner->GetID(), "Destroy"));
				}
			}
		}
	}
	return false;
}
/*
		// Get the location of the owner.
		const auto& location = m_pOwner->GetLocation();

		// We will update our internal rect at this point.
		m_transform.x = location.x - (m_transform.w / 2);
		m_transform.y = location.y - (m_transform.h / 2);
		m_transform.x -= cameraOffset.x;
		m_transform.y -= cameraOffset.y;

		int error = SDL_RenderCopyF(pRenderer, m_pTexture, NULL, &m_transform);
		if (error)
		{
			std::cout << "SDL_RenderCopyF. Error: " << SDL_GetError() << std::endl;
		}
*/
void PlayerMoveComponent::Update(double deltaTime)
{
	if (m_pImage == nullptr)
	{
		auto image = m_pOwner->GetComponent<Image>("Image");
		if (image != nullptr)
		{
			m_pImage = image;
			m_pImage->TurnOffRendering();
		}
	}
	// TODO(Shilo): Temp solution not final. currently hard coded maybe use an enum in terms of 
	// showing the difference between east and west etc.... // Current and Qued
	Vec2 deltaPosition{ 0,0 };
	bool recalc = false;
	auto& keyboard = EngineSingleton::Get()->GetKeyboard();
	// North
	if (keyboard.WasKeyPressed(SDL_KeyCode::SDLK_w)) // ask if this key was pressed. 
	{
		if (m_lastKeystroke != SDL_KeyCode::SDLK_w) // check to see if this key was pressed before. 
		{
			m_lastKeystroke = SDL_KeyCode::SDLK_w; // run this if we need to update last key. 
			recalc = true;
		}
	}
	// South
	if (keyboard.WasKeyPressed(SDL_KeyCode::SDLK_s))
	{
		if (m_lastKeystroke != SDL_KeyCode::SDLK_s) 
		{
			m_lastKeystroke = SDL_KeyCode::SDLK_s;
			recalc = true;
		}
	}
	// East
	if (keyboard.WasKeyPressed(SDL_KeyCode::SDLK_d))
	{
		if (m_lastKeystroke != SDL_KeyCode::SDLK_d)
		{
			m_lastKeystroke = SDL_KeyCode::SDLK_d;
			recalc = true;
		}
	}
	// West
	if (keyboard.WasKeyPressed(SDL_KeyCode::SDLK_a))
	{
		if (m_lastKeystroke != SDL_KeyCode::SDLK_a)
		{
			m_lastKeystroke = SDL_KeyCode::SDLK_a;
			recalc = true;
		}
	}
	
	//if (m_lastKeystroke == 0)
	//	return;
	Vec2 destLocation = m_pOwner->GetLocation();

	float alteringDelta = 16;
	 
		switch (m_lastKeystroke)
		{
		case SDL_KeyCode::SDLK_w:
			deltaPosition.y = -alteringDelta;
			break;
		case SDL_KeyCode::SDLK_s:
			deltaPosition.y = alteringDelta;
			break;
		case SDL_KeyCode::SDLK_a:
			deltaPosition.x = -alteringDelta;
			break;
		case SDL_KeyCode::SDLK_d:
			deltaPosition.x = alteringDelta;
			break;
		}

		destLocation.x += deltaPosition.x;
		destLocation.y += deltaPosition.y;
		StringId type = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(destLocation.x, destLocation.y);
		if (type != "wall") // TODO(Josh): Change this to a stringId comparison instead.
		{
			m_lastValidKeystroke = m_lastKeystroke;

			if (recalc)
			{
				m_path.clear();
			}
		}
		else
		{
			destLocation.x -= deltaPosition.x;
			destLocation.y -= deltaPosition.y;

			// making sure to reset the Delta position. 
			deltaPosition.x = 0;
			deltaPosition.y = 0;
			switch (m_lastValidKeystroke)
			{
			case SDL_KeyCode::SDLK_w:
				deltaPosition.y = -alteringDelta;
				break;
			case SDL_KeyCode::SDLK_s:
				deltaPosition.y = alteringDelta;
				break;
			case SDL_KeyCode::SDLK_a:
				deltaPosition.x = -alteringDelta;
				break;
			case SDL_KeyCode::SDLK_d:
				deltaPosition.x = alteringDelta;
				break;
			}

			destLocation.x += deltaPosition.x;
			destLocation.y += deltaPosition.y;
		}

		// Hallway transit via teleport. 
		if (destLocation.x < kLeftBounds)
		{
			m_pOwner->SetLocation(Vec2{ kFromLeftTeleport, destLocation.y });
			m_path.clear();
			return;
		}
		else if (destLocation.x > kRightBounds)
		{
			m_pOwner->SetLocation(Vec2{ kFromRightTeleport, destLocation.y });
			m_path.clear();
			return;
		}

		Vec2 locationBeforeMove = m_pOwner->GetLocation();

		// Here is where we finally account for delta time and make a movement. 
		if (!m_path.empty())
		{
			StepMovement(deltaTime);
		}
		else
		{
			// Get a new path. 
			auto path = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GeneratePathToBestTarget(m_pOwner->GetLocation(), destLocation);
			SetPath(path);
			if (!m_path.empty())
			{
				StepMovement(deltaTime);
			}
		}

		// Here we calculate which way we have traveled so that we can render in the correct orientation.
		Vec2 locationAfterMove = m_pOwner->GetLocation();
		m_changeInMove.x = 0;
		m_changeInMove.y = 0;
		m_changeInMove = locationAfterMove - locationBeforeMove;

		if (m_lastKeystroke == 0)
			return;

		// The gap beween 0.01 and -0.01 is intentional. 
		if (m_changeInMove.x > 0.01)
		{
			m_rightMove = true;
			m_upMove = false;
			m_downMove = false;
			m_leftMove = false;
			//std::cout << "moving right!\n" << m_changeInMove.x << " :: " << m_changeInMove.y << "\n";
		}
		else if (m_changeInMove.x < -0.01)
		{
			m_rightMove = false;
			m_upMove = false;
			m_downMove = false;
			m_leftMove = true;
			//std::cout << "moving left!\n" << m_changeInMove.x << " :: " << m_changeInMove.y << "\n";
		}
		else if (m_changeInMove.y > 0.01)
		{
			m_rightMove = false;
			m_upMove = false;
			m_downMove = true;
			m_leftMove = false;
			//std::cout << "moving down!\n" << m_changeInMove.x << " :: " << m_changeInMove.y << "\n";
		}
		else if (m_changeInMove.y < -0.01)
		{
			m_rightMove = false;
			m_upMove = true;
			m_downMove = false;
			m_leftMove = false;
			//std::cout << "moving up!\n" << m_changeInMove.x << " :: " << m_changeInMove.y << "\n";
		}
		//m_lastLocation = m_pOwner->GetLocation();
	
}

void PlayerMoveComponent::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	// Get the location of the owner.
	const auto& location = m_pOwner->GetLocation();

	// We will update our internal rect at this point.
	m_pImage->m_transform.x = location.x - (m_pImage->m_transform.w / 2);
	m_pImage->m_transform.y = location.y - (m_pImage->m_transform.h / 2);
	m_pImage->m_transform.x -= cameraOffset.x;
	m_pImage->m_transform.y -= cameraOffset.y;

	if (m_upMove)
	{
		int error = SDL_RenderCopyExF(pRenderer, m_pImage->m_pTexture, NULL, &m_pImage->m_transform, -90, NULL, SDL_FLIP_NONE);
		if (error)
		{
			std::cout << "SDL_RenderCopyExF. Error: " << SDL_GetError() << std::endl;
		}
	}
	if (m_downMove) 
	{
		int error = SDL_RenderCopyExF(pRenderer, m_pImage->m_pTexture, NULL, &m_pImage->m_transform, 90, NULL, SDL_FLIP_NONE);
		if (error)
		{
			std::cout << "SDL_RenderCopyExF. Error: " << SDL_GetError() << std::endl;
		}
	}
	if (m_leftMove) 
	{
		int error = SDL_RenderCopyExF(pRenderer, m_pImage->m_pTexture, NULL, &m_pImage->m_transform, 0, NULL, SDL_FLIP_HORIZONTAL);
		if (error)
		{
			std::cout << "SDL_RenderCopyExF. Error: " << SDL_GetError() << std::endl;
		}
	}
	if (m_rightMove) 
	{
		int error = SDL_RenderCopyF(pRenderer, m_pImage->m_pTexture, NULL, &m_pImage->m_transform);
		if (error)
		{
			std::cout << "SDL_RenderCopyF. Error: " << SDL_GetError() << std::endl;
		}
	}
}

void PlayerMoveComponent::SetPath(const Plan& path)
{
	m_path.clear();
	m_path = path;
}

void PlayerMoveComponent::StepMovement(double deltaTimeMs)
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
