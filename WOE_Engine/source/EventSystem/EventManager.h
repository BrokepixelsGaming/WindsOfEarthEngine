#pragma once

#include <deque>
#include <unordered_map>

#include "EventSystem\Event.h"
#include "EntitySystem\Entity.h"

//class Event;

class EventManager
{
private:
	// some container of events. 
	std::deque<Event> m_EventList;

	// some container of Listeners
	std::unordered_map<EntityID, Entity*> m_ListenersList;

public:

	bool Init();
	void QueueEvent(Event evt);
	bool ProcessEvents();
	bool ProcessAndQueueSDLEvents();
	void AddListener(Entity* listenerToAdd);
	void RemoveListener(Entity* listenerToRemove);
};