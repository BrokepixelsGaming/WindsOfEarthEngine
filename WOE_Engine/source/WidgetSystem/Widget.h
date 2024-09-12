#pragma once

#include "../tools/BleachNew.h"
#include <string>
#include"SDL/SDL.h"




class Widget
{
public:
	virtual ~Widget() {}
	// Gets called by thge main loop each frame to render this object. 
	virtual void Render([[maybe_unused]] SDL_Renderer* pRenderer) {};

	// Handles this event in the context of this object.
	virtual void HandleEvent([[maybe_unused]] SDL_Event* pEvent) {};

	// Returns a readable name.
	virtual const std::string GetName() { return std::string(); };

	// Gets called by the main loop each frame to update this object's state. 
	virtual void Update([[maybe_unused]] double deltaTime) {};

};