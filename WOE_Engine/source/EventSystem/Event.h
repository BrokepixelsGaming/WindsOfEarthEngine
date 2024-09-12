#pragma once
#include <string>

#include "config\Settings.h"
#include "SDL/SDL.h"

class Event
{
	// Event enums aka event types?This should be a method for Managers to talk to others. I think for input base it 
	// off the ascii table c++ to map keys to processes, should also be able to carry a EntityID and a string(for maybe a way to create entities?
public:
	enum class EventType
	{
		Quit,
		Input,
		DestroyEntity,
		Creation,
		Collision,
		Place
	};
	Event(EventType evtType, EntityID targetEntityID, const char* command);
	Event(EventType evtType, EntityID targetEntityID, const char* command, SDL_Event evt);
	const EventType GetEventType() { return m_evtType; }
	const EntityID GetTargetEntityID() { return m_targetEntityId; }
	const std::string GetCommandString() { return m_commandString; }
	const SDL_Event* GetSDLEvent() { return &m_sdlEvent; }

private:

	const std::string m_commandString; // allows for "Command strings" to pass from one manager to another. example "Create Orc" 
	EntityID m_targetEntityId; // Id of the intended target of the event.
	EventType m_evtType; // enum so that things outside of event can know what kind of event is intended.
	SDL_Event m_sdlEvent;
};

