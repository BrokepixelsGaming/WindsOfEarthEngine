#pragma once
#include "ComponentSystem/Component.h"
#include "tools/Utilities/tinyxml2.h"
#include "EntitySystem/Entity.h"

class GameComponentFactory
{

public:
	Component* ParseComponent(const tinyxml2::XMLElement* Component, Entity* pOwner );
};