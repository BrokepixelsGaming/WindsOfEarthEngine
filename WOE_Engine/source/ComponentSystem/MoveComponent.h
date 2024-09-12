#pragma once
#include <array>

#include "ComponentSystem/Component.h"
#include "EntitySystem/Entity.h"
#include "config/Settings.h"


///////////////////////////////////////////////////////////////
// Used to control a entity via inputs. 
///////////////////////////////////////////////////////////////
class MoveComponent :
	public Component
{
protected:
	Entity* m_pOwner;

public:
	// Constructor passing in the entity owner. Generally only the player will ever have a move Component. 
	// If an NPC needed to move another component should be implemented. As this is tightly coupled to keyboard movements. 
	// may consider renaming this component to "Input" or something.
	MoveComponent(Entity* owner);

	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	
	void Update(double deltaTime) override;

};