#pragma once

//---------------------------------------------------------------------------------------------------------------------
// The camera is just a data class used as an offset to pass to the renderer.
// Note:
//		- The camera is referenced in RenderRect.cpp and TileDef.cpp
// 
// Known issues:
//		- Buttons follow the camera
//		- Placement of entities does not follow cursor
//			- Needs to be offset with GetWorldXRelativeToCamera()
//	
//---------------------------------------------------------------------------------------------------------------------
class Event;
union SDL_Event;

class Camera
{
private:
	// The offset to render by
	float m_renderOffsetX;
	float m_renderOffsetY;

	// Camera speed
	float m_cameraSpeed;
	
	// Dragging utility
	int m_cursorX;
	int m_cursorY;
	float m_prevOffsetX;
	float m_prevOffsetY;
	bool m_isDragging;

public:
	Camera();

	// Update the camera
	void Update(double deltaTime);

	// Process events
	void ProcessSDLEvent(SDL_Event& evt);

	// Checks to see if the rendarable is inside the camera.
	bool IsInCamera(int x, int y, int width, int height);

	void ResetCamera();

	// Getters
	int GetOffsetX() { return (int)m_renderOffsetX; }
	int GetOffsetY() { return (int)m_renderOffsetY; }

	// Gets a position relative to the camera location. Used to put objects in actual world space.
	float GetWorldXRelativeToCamera(float x) { return x + m_renderOffsetX; }
	float GetWorldYRelativeToCamera(float y) { return y + m_renderOffsetY; }
};