#include "ProjectileComponent.h"
#include "EngineInterface/EngineSingleton.h"
#include "EventSystem/Event.h"
#include "ComponentSystem/Collision.h"

void ProjectileComponent::Update(double deltaTime)
{
	m_lifetime -= deltaTime;
	if (m_lifetime < 0)
	{
		//destroy self. 
		EngineSingleton::Get()->GetEventManager()->QueueEvent(Event(Event::EventType::DestroyEntity, m_pOwner->GetID(), "Self Destroy"));
	}
	StepMovement(deltaTime);
}

bool ProjectileComponent::ProcessEvent(Event& evt)
{
	if (evt.GetTargetEntityID() == m_pOwner->GetID())
	{
		if (evt.GetEventType() == Event::EventType::Collision)
		{
			if(evt.GetCommandString() != m_pOwner->GetComponent<Collision>("Collision")->GetMessage())
				EngineSingleton::Get()->GetEventManager()->QueueEvent(Event(Event::EventType::DestroyEntity, m_pOwner->GetID(), "Self Destroy"));
		}
	}
	return false;
}

void ProjectileComponent::StepMovement(double deltaTime)
{
	m_pOwner->UpdateLocation(Vec2(m_direction.x * m_speed * (float)deltaTime, m_direction.y * m_speed * (float)deltaTime));
}
