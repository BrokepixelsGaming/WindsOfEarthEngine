#pragma once
#include "Component.h"
#include <map>
#include "..\3rdParty\include\SDL\SDL.h"

class Animation :
    public Component
{
protected:
	StringId m_type = "Animation";
private:
	struct AnimationSequence
	{
	public:
		// Name of the animation
		std::string m_name;

		// Starting point of the animation. 
		int m_firstFrame;

		// Ending point of the animation. 
		int m_lastFrame;
	};

private:
	// How fast the animation goes.
	const double m_kFrameRate;

	// How many frames in a single row.
	int m_numSpriteSheetColumns;

	// The sprite sheet itself.
	SDL_Texture* m_pSpriteSheet;

	// Object's location on screen. 
	SDL_Rect m_transform;

	// Area of the sprite to render.
	SDL_Rect m_sourceTransform;

	// Container for the animation to hold the different animations. 
	std::map<std::string, AnimationSequence> m_allAnimations;

	// Hold the name of the current sequence that is being use.
	std::string m_currentSequenceName;

	int m_currentFrame;

	double m_frameTime;

	int m_xOffset;

	int m_yOffset;

public:
	Animation(const char* pSpriteSheetPath, double frameRate,
		int frameWidth, int frameHeight, SDL_Renderer* pRenderer,
		[[maybe_unused]] int xOffset, [[maybe_unused]] int yOffset);
	~Animation();

	void AddAnimationSequence(std::string, int firstFrame, int lastFrame);

	void PlayAnimation(std::string sequenceName);

	void ResetFrameTime();

	void UpdateSourceTransform();

	void Update(double deltaTime);

	void Render(SDL_Renderer* pRenderer, SDL_FRect m_transform, Vec2 cameraOffset);
};

