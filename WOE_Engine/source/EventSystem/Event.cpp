#include "Event.h"


Event::Event(EventType evtType, EntityID targetEntityID, const char* command)
	: m_evtType(evtType)
	, m_targetEntityId(targetEntityID)
	, m_commandString(command)
	, m_sdlEvent()
{
}
Event::Event(EventType evtType, EntityID targetEntityID, const char* command, SDL_Event evt)
	: m_evtType(evtType)
	, m_targetEntityId(targetEntityID)
	, m_commandString(command)
	, m_sdlEvent(evt)
{
}
