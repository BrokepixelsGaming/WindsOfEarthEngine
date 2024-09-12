#include "ButtonComponent.h"

#include "EventSystem/Event.h"
#include "EngineInterface/EngineSingleton.h"

#include "BuildComponent.h"

ButtonComponent::ButtonComponent(Entity* owner, Vec2 size)
	: InteractableComponent(owner,size)
	, m_isHighlighted(false)
{
}

ButtonComponent::~ButtonComponent()
{
}

bool ButtonComponent::ProcessEvent(Event& evt)
{
	switch (evt.GetSDLEvent()->type)
	{
	case SDL_MOUSEMOTION:
		m_isHighlighted = HitTest(evt.GetSDLEvent()->button.x, evt.GetSDLEvent()->button.y);
		// used with a hit test to "highlight" a button/unit
		// 
		// TODO(Andrew) : create a basic png resource type and use 2 for highlited and un-highlited buttons
		break;
	case SDL_MOUSEBUTTONUP:	// TODO(Andrew): determin which mouse button was used
		if (HitTest(evt.GetSDLEvent()->button.x, evt.GetSDLEvent()->button.y))
		{
			BuildComponent* pPointer = m_pOwner->GetComponent<BuildComponent>("Build");
			pPointer->SetTryToBuild(true);
			//EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->CreateEntity("Ghost");
			//std::cerr << "Button Pressed Test" << "\n";

			//Do whatever needs to happen when button/unit is selected
			//TODO(Andrew) : create somekind of call/funtion/event to use this
		}
		break;
	default:
		break;
	}

	return true;
}

void ButtonComponent::Update([[maybe_unused]]double deltaTime)
{
	if (m_isHighlighted == true)
	{
		//EngineSingleton::Get()->GetEventManager()->QueueEvent(Event(Event::EventType::Input, m_pOwner->GetID(), "Input"));
	}
}

bool ButtonComponent::HitTest(int x, int y)
{
	return(x >= m_pOwner->GetLocation().x - m_size.x / 2 && x <= m_pOwner->GetLocation().x + m_size.x / 2 &&
		y >= m_pOwner->GetLocation().y - m_size.y / 2 && y <= m_pOwner->GetLocation().y + m_size.y / 2);
}

