#include "Camera.h"
#include "EngineInterface/EngineSingleton.h"
#include "EventSystem/Event.h"
#include "SDL/SDL.h"
#include "config/Settings.h"
#include "WorldSystem/World.h"
#include "TileMapSystem/TileMap.h"

#define MOUSE_DRAG 1 // Change this to 1 for mouse drag camera

Camera::Camera()
	: m_renderOffsetX(0)
	, m_renderOffsetY(0)
	, m_cameraSpeed(100.0f) // 100 px per second
	, m_cursorX(0)
	, m_cursorY(0)
	, m_isDragging(false)
	, m_prevOffsetX(0.0f)
	, m_prevOffsetY(0.0f)
{
}

//---------------------------------------------------------------------------------------------------------------------
// Update the camera
//---------------------------------------------------------------------------------------------------------------------
void Camera::Update([[maybe_unused]]double deltaTime)
{
#if !MOUSE_DRAG

	// Get keyboard
	Keyboard& keyboard = EngineSingleton::Get()->GetKeyboard();

	if (keyboard.IsKeyDown(SDL_KeyCode::SDLK_w))
	{
		m_renderOffsetY -= m_cameraSpeed * deltaTime;
	}
	if (keyboard.IsKeyDown(SDL_KeyCode::SDLK_a))
	{
		m_renderOffsetX -= m_cameraSpeed * deltaTime;
	}
	if (keyboard.IsKeyDown(SDL_KeyCode::SDLK_s))
	{
		m_renderOffsetY += m_cameraSpeed * deltaTime;
	}
	if (keyboard.IsKeyDown(SDL_KeyCode::SDLK_d))
	{
		m_renderOffsetX += m_cameraSpeed * deltaTime;
	}
#endif
}

//---------------------------------------------------------------------------------------------------------------------
// Process events.
//---------------------------------------------------------------------------------------------------------------------
void Camera::ProcessSDLEvent(SDL_Event& evt)
{
#if MOUSE_DRAG

	switch (evt.type)
	{
	case SDL_MOUSEMOTION:
	{
		// If the map is being dragged,
		if (m_isDragging)
		{
			if (!kXAxisLocked) 
			{
				// Get the base x and y values of the mouse, then get the current x and y values of the mouse.
				float differenceX = m_cursorX - static_cast<float>(evt.button.x); // Subtract them to get the difference
				// Add the difference to our current offset
				m_renderOffsetX = differenceX + m_prevOffsetX; // Also add the offset that was previously saved on mouse release

				if (m_renderOffsetX > kXMax)
				{
					m_renderOffsetX = kXMax;
				}
				if (m_renderOffsetX < kXMin)
				{
					m_renderOffsetX = kXMin;
				}
			}
			if (!kYAxisLocked)
			{
				float differenceY = m_cursorY - static_cast<float>(evt.button.y);
				m_renderOffsetY = differenceY + m_prevOffsetY;
				if (m_renderOffsetY > kYMax)
				{
					m_renderOffsetY = kYMax;
				}
				if (m_renderOffsetY < kYMin)
				{
					m_renderOffsetY = kYMin;
				}
			}

		}
		break;
	}
	// Mouse hold
	case SDL_MOUSEBUTTONDOWN:
	{
		if (evt.button.button == SDL_BUTTON_RIGHT)
		{
			// First, get the current x and y values of the mouse and save it
			m_cursorX = evt.button.x;
			m_cursorY = evt.button.y;

			// Set is dragging to true
			m_isDragging = true;
		}
		break;
	}
	// Mouse release
	case SDL_MOUSEBUTTONUP:
	{
		if (evt.button.button == SDL_BUTTON_RIGHT)
		{
			// Set dragging to false
			m_isDragging = false;

			// Save the offset values to use for the next camera drag
			m_prevOffsetX = m_renderOffsetX;
			m_prevOffsetY = m_renderOffsetY;
		}
		break;
	}
	}
#endif
}

//---------------------------------------------------------------------------------------------------------------------
// Checks to see if the rendarable is inside the camera.
//---------------------------------------------------------------------------------------------------------------------
bool Camera::IsInCamera(int x, int y, int width, int height)
{
	// Check if it's off screen
	if (x - m_renderOffsetX < - width || y - m_renderOffsetY < - height ||
		x - m_renderOffsetX > kMaxWindowWidth || y - m_renderOffsetY > kMaxWindowHeight)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Camera::ResetCamera()
{
	m_renderOffsetX = 0;
	m_renderOffsetY = 0;
	m_prevOffsetX = 0;
	m_prevOffsetY = 0;

	
}
