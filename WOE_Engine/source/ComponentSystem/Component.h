#pragma once
#include "EventSystem/Event.h"
#include "SDL/SDL.h"
#include "tools/Utilities/StringId.h"
#include "tools/Utilities/Vector.h"
//#include "EntitySystem/Entity.h"

struct SDL_Renderer;

///////////////////////////////////////////////////////////////
// Pure Virtual Class. 
// Base class for all components that may be attached to an object such as 
// Audio, Animation, Collision or any other types that may come up with. 
///////////////////////////////////////////////////////////////
class Component
{
protected:
	StringId m_type = "Component";
public:
	virtual ~Component() = default;
	virtual void Update([[maybe_unused]] double deltaTime){};
	virtual bool ProcessEvent([[maybe_unused]] Event& evt) { return false; };
	virtual bool ProcessEvent([[maybe_unused]] SDL_Event& evt) { return false; };
	virtual void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]]Vec2 cameraOffset) {};
	virtual StringId GetType() { return m_type; }
};