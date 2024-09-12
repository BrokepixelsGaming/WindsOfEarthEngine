#include "CollectorComponent.h"
#include "EngineInterface/EngineSingleton.h"
#include <iostream>
#include "../tools/Utilities/StringId.h"

CollectorComponent::CollectorComponent(Entity* owner)
	: m_pOwner(owner)
	, m_count(0)
	, m_collectionSFX(BLEACH_NEW(Audio))
	, m_powerUpMusic(BLEACH_NEW(Audio))
{
	m_collectionSFX->Load("data/Audio/WAV/pacman_chomp.wav");
	m_powerUpMusic->Load("data/Audio/MP3/134- Earthbound - The Jolly Flying Man.mp3");
	m_collectionSFX->SetVolume(50);
}

CollectorComponent::~CollectorComponent()
{
	BLEACH_DELETE(m_collectionSFX);
	BLEACH_DELETE(m_powerUpMusic);
}

void CollectorComponent::Update([[maybe_unused]]double deltaTime)
{

	// look at the tile below us. 
	Vec2 currentLocation = m_pOwner->GetLocation();
	// This is based off of the images being 15 x 15.Ideally this should be calculated but I didn't want to incur the cost. of figuring it out every cycle. 
	// for now this will be made into a constexpr. 
	currentLocation.x -= kRectCenteringModifier;
	currentLocation.y -= kRectCenteringModifier;
	// if the tile has an collectable of any kind pick it up
	bool collectable = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->TileHasOverlay(currentLocation.x, currentLocation.y);
	// remove it from the map. (function call to the map)
	if (collectable)
	{	
		StringId type = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileOverlayTypeByLocation(currentLocation.x, currentLocation.y);

		if (type == "largeOrb") // TODO(Josh): Change this to a stringId comparison instead.
		{
			m_count += kLargeOrbPoints;
			EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetPowerUp(true);
			EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTimer( g_timerLife);
			std::cout << "You've collected a powerUp go eat some ghost. \n";
			m_powerUpMusic->PlaySound();
			m_collectionSFX->PlaySoundOverride();
		}
		if (type == "smallOrb")
		{
			m_count += kSmallOrbPoints;
			m_collectionSFX->PlaySoundOverride();
		}
		// call tile map to erase the orb. 
		EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->CollectLocation(currentLocation);
		if (!EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->CollectableRemaining())
		{
			std::cout << "You've collected all the orbs you win!\n";
			//EngineSingleton::Get()->GetEventManager()->QueueEvent(Event(Event::EventType::Quit, 0, "Quit"));
		}
	}

	if (EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->PowerUp())
	{
		EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTimer(deltaTime * -1);
		if (EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->TimeUp())
		{
			EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetPowerUp(false);
			std::cout << "PowerUp Time is OVER!\n";
			m_powerUpMusic->StopSound();
		}
	}
}
