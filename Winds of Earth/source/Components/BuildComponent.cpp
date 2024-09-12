#include "BuildComponent.h"
#include "EventSystem/Event.h"
#include "EngineInterface/EngineSingleton.h"
#include "config/Settings.h"

#include "SpawnerComponent.h"

BuildComponent::BuildComponent(Entity* owner)
	:m_pOwner(owner)
	,m_canTryToBuild(false)
{
}

BuildComponent::~BuildComponent()
{
}

bool BuildComponent::ProcessEvent(Event& evt)
{
	switch (evt.GetSDLEvent()->type)
	{
	case SDL_MOUSEBUTTONDOWN:	// TODO(Andrew): determin which mouse button was used
		if (HitTest(evt.GetSDLEvent()->button.x, evt.GetSDLEvent()->button.y))
		{
			if (m_canTryToBuild == true)
			{
				float centerX = static_cast<float>(EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SnapX(evt.GetSDLEvent()->button.x));
				float centerY = static_cast<float>(EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SnapY(evt.GetSDLEvent()->button.y));

				if(EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileAvailabilityByLocation(centerX, centerY) == true)
				{
					SpawnerComponent* pPointer = m_pOwner->GetComponent<SpawnerComponent>("Spawner");
					pPointer->SpawnEntity(static_cast<int>(centerX), static_cast<int>(centerY));
					EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileAvailabilityByLocation(centerX, centerY, false);
					m_canTryToBuild = false;
				}
				else
				{
					//std::cerr << "Can't build their!" << "\n";
				}
			}

			//Do whatever needs to happen when button/unit is selected
			//TODO(Andrew) : create somekind of call/funtion/event to use this
		}
		break;
	default:
		break;
	}

	return true;
}

void BuildComponent::SetTryToBuild(bool state)
{
	//std::cerr << "try to build?" << "\n";
	m_canTryToBuild = state;
}

bool BuildComponent::HitTest(int x, int y)
{
	//TODO(Andrew): remove mangic numbers once corrections to hit test can be made
	return(x >= 0 && x <= kMaxWindowWidth &&
		y >= 0 && y <= kMaxWindowHeight);
}
