#include "EventManager.h"
#include "Event.h"

#include <iostream>
#include "..\3rdParty\include\SDL\SDL.h"
#include "EngineInterface/EngineSingleton.h"
#define _LOGGING_ 0

static constexpr float kRenderScaleUpperLimit = 5;
static constexpr float kRenderScaleLowerLimit = .1f;
float renderScale = 1.0f;

bool EventManager::Init()
{
	return true;
}

void EventManager::QueueEvent(Event evt)
{
	m_EventList.push_back(evt);
}

// Returns true if it's time to quit. 
bool EventManager::ProcessEvents()
{
	std::deque<Event> m_trashcan;
	bool result;
	while (!m_EventList.empty()) 
	{
	
		Event evt = m_EventList.front();
		m_EventList.pop_front();
		if (evt.GetEventType() == Event::EventType::Quit)
		{
			return true;
		}
		if (evt.GetEventType() == Event::EventType::DestroyEntity)
		{
			m_trashcan.push_back(evt);
			continue;
		}
		if (evt.GetEventType() == Event::EventType::Creation)
		{
			EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->CreateEntity(evt.GetCommandString());
		}
		for (auto& listner : m_ListenersList)
		{
#if _LOGGING_
			std::cout << "EntityID:: " << listner.second->GetID() << " \n";
			std::cout << "Event:: ";
			switch (evt.GetEventType())
			{
			case Event::EventType::DestroyEntity:
			{
				std::cout << " Type :: DestroyEntity :: ";
				break;
			}
			case Event::EventType::Collision:
			{
				std::cout << " Type :: Collision :: ";
				break;
			}
			case Event::EventType::Creation:
			{
				std::cout << " Type :: Creation :: ";
				break;
			}
			case Event::EventType::Input:
			{
				std::cout << " Type :: Input :: ";
				break;
			}
			case Event::EventType::Quit:
			{
				std::cout << " Type :: Quit :: ";
				break;
			}
			default:
				std::cout << " Type :: Invalid :: ";
				break;
			}

			std::cout << "Command :: " << evt.GetCommandString() << " ";
			std::cout << "TargetID :: " << evt.GetTargetEntityID() << "\n";
#endif
			result = listner.second->ProcessEvent(evt);

			if (result)
				return true;
		}
	}
	while (!m_trashcan.empty())
	{
		Event evt = m_trashcan.front();
		m_trashcan.pop_front();
#if _LOGGING_
		std::cout << "Event:: ";
		switch (evt.GetEventType())
		{
		case Event::EventType::DestroyEntity:
		{
			std::cout << " Type :: DestroyEntity :: ";
			break;
		}
		case Event::EventType::Collision:
		{
			std::cout << " Type :: Collision :: ";
			break;
		}
		case Event::EventType::Creation:
		{
			std::cout << " Type :: Creation :: ";
			break;
		}
		case Event::EventType::Input:
		{
			std::cout << " Type :: Input :: ";
			break;
		}
		case Event::EventType::Quit:
		{
			std::cout << " Type :: Quit :: ";
			break;
		}
		default:
			std::cout << " Type :: Invalid :: ";
			break;
		}

		std::cout << "Command :: " << evt.GetCommandString() << " ";
		std::cout << "TargetID :: " << evt.GetTargetEntityID() << "\n";
#endif
		for (auto& listner : m_ListenersList)
		{
			result = listner.second->ProcessEvent(evt);
		}
		EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->DestroyEntity(evt.GetTargetEntityID());
	}
	return false;
}

bool EventManager::ProcessAndQueueSDLEvents()
{
	SDL_Event evt;
	auto* engine = EngineSingleton::Get();
	auto& keyboard = engine->GetKeyboard();
	
	while (SDL_PollEvent(&evt) != 0)
	{
		// If window event is a closure (X).
		// We Check input and then we Submit the event to our EventSystem for the things we care about and Any key presses will be passed 
		// to the keyboard to hold the state from SDL. 
		// passing the event where it needs to go. 
		switch(evt.type)
		{
			case SDL_WINDOWEVENT:
			{
				if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					return true;
				}
			}
			case SDL_MOUSEWHEEL:
			{
				std::cout << "Mouse Wheel Direction Event X->" << evt.wheel.x << " Y ->" << evt.wheel.y<< "\n";
				if (evt.wheel.y > 0)
				{
					renderScale += .1f;
				}
				else if (evt.wheel.y < 0)
				{
					renderScale += -.1f;
				}

				if (renderScale > kRenderScaleUpperLimit)
					renderScale = kRenderScaleUpperLimit;
				else if (renderScale < kRenderScaleLowerLimit)
					renderScale = kRenderScaleLowerLimit;

				//SDL_RenderSetScale(EngineSingleton::Get()->GetWindow()->GetRenderer(), renderScale, renderScale);
			}
			case SDL_KEYDOWN:
			{
				if (!keyboard.IsKeyDown(evt.key.keysym.sym)) 
				{
					keyboard.SetKeyState(evt.key.keysym.sym, true);
#if _LOGGING_
					std::cout << evt.key.keysym.sym <<" KeyDown\n";
#endif
				}
				// Adding an additional way to close the game. 
				if (evt.key.keysym.sym == SDLK_ESCAPE)
				{
					return true;
				}
				break;

			}
			case SDL_KEYUP:
			{
				keyboard.SetKeyState(evt.key.keysym.sym, false);
				//std::cout << "KeyUP\n";
				break;
			}
			default:
			{
				engine->GetEventManager()->QueueEvent(Event(Event::EventType::Input, 0, "Input", evt));
				
				// The camera only listens for these events
				if (evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEBUTTONUP || evt.type == SDL_MOUSEMOTION)
				{
					engine->GetCamera().ProcessSDLEvent(evt);
				}
			}
		}
	}
	return false;
}

void EventManager::AddListener(Entity* listenerToAdd)
{
	m_ListenersList.emplace(listenerToAdd->GetID(), listenerToAdd);
}

void EventManager::RemoveListener([[maybe_unused]]Entity* listenerToRemove)
{
	 m_ListenersList.erase(listenerToRemove->GetID());
}
