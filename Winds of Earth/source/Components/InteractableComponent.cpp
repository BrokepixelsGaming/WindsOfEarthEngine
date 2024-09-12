#include "InteractableComponent.h"

#include "EventSystem/Event.h"
#include "EngineInterface/EngineSingleton.h"

#include "RenderSystem/Camera.h"
#include "SpawnerComponent.h"

InteractableComponent::InteractableComponent(Entity* owner, Vec2 size)
	:m_pOwner(owner)
	,m_size(size)
	,m_spawnDelay(true)
{
}

InteractableComponent::~InteractableComponent()
{
}

bool InteractableComponent::ProcessEvent(Event& evt)
{
	switch (evt.GetSDLEvent()->type)
	{
	case SDL_MOUSEMOTION:
		//m_isHighlighted = HitTest(evt.GetSDLEvent()->button.x, evt.GetSDLEvent()->button.y);
			// used with a hit test to "highlight" a button/unit
			// 
			// TODO(Andrew) : create a basic png resource type and use 2 for highlited and un-highlited buttons
		break;
	case SDL_MOUSEBUTTONUP:	// TODO(Andrew): determin which mouse button was used
		if (HitTest(evt.GetSDLEvent()->button.x, evt.GetSDLEvent()->button.y))
		{
			if (m_spawnDelay == true)
			{
				m_spawnDelay = false;
			}
			else
			{
				SpawnerComponent* pPointer = m_pOwner->GetComponent<SpawnerComponent>("Spawner");
				pPointer->SpawnEntity((static_cast<int>(m_pOwner->GetLocation().x + m_size.x)), static_cast<int>(m_pOwner->GetLocation().y));
				//pPointer->SetTryToBuild(true);
				//EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->CreateEntity("Ghost");
				//std::cerr << "Building Pressed Test" << "\n";

				//Do whatever needs to happen when button/unit is selected
				//TODO(Andrew) : create somekind of call/funtion/event to use this
			}
		}
		break;
	default:
		break;
	}

	return true;
}

void InteractableComponent::Update([[maybe_unused]]double deltaTime)
{
	
}

bool InteractableComponent::HitTest(int x, int y)
{
	auto& camera = EngineSingleton::Get()->GetCamera();

	float offsetLocationX = (m_pOwner->GetLocation().x - camera.GetOffsetX());
	float offsetLocationY = (m_pOwner->GetLocation().y - camera.GetOffsetY());

	return(x >= offsetLocationX - m_size.x /2 && x <= offsetLocationX + m_size.x / 2 &&
		y >= offsetLocationY - m_size.y / 2 && y <= offsetLocationY + m_size.y / 2);
}
