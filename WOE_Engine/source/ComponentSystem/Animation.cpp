#include "Animation.h"

#include <..\..\3rdParty\include\SDL\SDL_image.h>
#include <assert.h>

#include "EngineInterface/EngineSingleton.h"

Animation::Animation(const char* pSpriteSheetPath, double frameRate, int frameWidth, int frameHeight, SDL_Renderer* pRenderer, [[maybe_unused]] int xOffset, [[maybe_unused]] int yOffset)
	: m_kFrameRate(frameRate)
	, m_pSpriteSheet(nullptr)
	, m_currentFrame(-1)
	, m_frameTime(0)
	, m_numSpriteSheetColumns(0)
	, m_xOffset(xOffset)
	, m_yOffset(yOffset)
{
	// Attempt to load the image to a surface.
	SDL_Surface* pSpriteSheetSurface = IMG_Load(pSpriteSheetPath);
	//auto& lua = EngineSingleton::Get()->GetLuaState();
	// Error checking 
	if (pSpriteSheetSurface == nullptr)
	{
		
		//lua.DoString("failed to Get Surface. Error:");
		//lua.DoString( SDL_GetError());
		return;
	}

	// Calculate the number of columns.
	m_numSpriteSheetColumns = pSpriteSheetSurface->w / frameWidth;
	assert(m_numSpriteSheetColumns != 0); // Because this should never = zero. 

	// Initialize the transform.
	m_transform.x = 0;
	m_transform.y = 0;
	m_transform.w = frameWidth;
	m_transform.h = frameHeight;

	// Initialize the source transform.
	m_sourceTransform = m_transform;

	// Create the texture.
	m_pSpriteSheet = SDL_CreateTextureFromSurface(pRenderer, pSpriteSheetSurface);

	// Error check.
	if (m_pSpriteSheet == nullptr)
	{

		//lua.DoString("failed to Get Texture. Error:");
		//lua.DoString(SDL_GetError());
		return;
	}

	// Free the surface. 
	SDL_FreeSurface(pSpriteSheetSurface);

	ResetFrameTime();
}

Animation::~Animation()
{
	// Destroy the texture. 
	SDL_DestroyTexture(m_pSpriteSheet);
}

void Animation::AddAnimationSequence(std::string name, int firstFrame, int lastFrame)
{
	// Create animation sequence.
	m_allAnimations[name] = AnimationSequence{ name, firstFrame, lastFrame };
}

void Animation::PlayAnimation(std::string sequenceName)
{
	// If this particular sequence is already playing or hasn't been added, skip.
	if (sequenceName == m_currentSequenceName || // Already playing
		m_allAnimations.find(sequenceName) == m_allAnimations.end()) // Doesn't exist.
	{
		return;
	}

	// Access the sequence with this name and set its first frame.
	AnimationSequence& newSequence = m_allAnimations[sequenceName];
	m_currentFrame = newSequence.m_firstFrame;

	// Update current sequence name, frame time and source transfrom. 
	m_currentSequenceName = sequenceName;
	ResetFrameTime();
	UpdateSourceTransform();
}

void Animation::ResetFrameTime()
{
	// How many seconds the current frame. 
	m_frameTime = 1. / m_kFrameRate;
}

///////////////////////////////////////////////////////////////
// How it shifts from one sprite to the next. 
///////////////////////////////////////////////////////////////
void Animation::UpdateSourceTransform()
{
	int frameX = m_currentFrame % m_numSpriteSheetColumns;
	int frameY = m_currentFrame / m_numSpriteSheetColumns;
	m_sourceTransform.x = (frameX * m_sourceTransform.w) + m_xOffset;
	m_sourceTransform.y = (frameY * m_sourceTransform.h) + m_yOffset;
}

///////////////////////////////////////////////////////////////
// Figures out which sprite it needs to play for animation.
///////////////////////////////////////////////////////////////
void Animation::Update(double deltaTime)
{
	// See if an animation is playing.
	if (m_currentFrame == -1)
	{
		return;
	}

	// Access the current animation sequence.
	AnimationSequence& currentSequence = m_allAnimations[m_currentSequenceName];

	// Allocate a dirty flag to see if the frame number changes.
	bool frameIsDirty = false;

	// As long as delta time is large, update frame.
	while (deltaTime > m_frameTime)
	{
		frameIsDirty = true;

		deltaTime -= m_frameTime;
		ResetFrameTime();

		// Check for Looping.
		if (currentSequence.m_lastFrame == m_currentFrame) // If on the last frame
		{
			m_currentFrame = currentSequence.m_firstFrame; // Loop back to first frame.
		}
		else
		{
			++m_currentFrame; // Else progress on. 
		}
	}

	// After the frame has been updated, subtract delta time. 
	m_frameTime -= deltaTime;

	// If frame is dirty, update source transform.
	if (frameIsDirty)
	{
		UpdateSourceTransform();
	}
}

///////////////////////////////////////////////////////////////
// Uses SDL_RenderCopyF to render the animation to the window.  
///////////////////////////////////////////////////////////////
void Animation::Render(SDL_Renderer* pRenderer, SDL_FRect transform, [[maybe_unused]]Vec2 cameraOffset)
{

	// Get the location of the owner. 
	auto location = transform;

	// We will update our internal rect at this point.
	m_transform.x = static_cast<int>(location.x - (m_transform.w / 2));
	m_transform.y = static_cast<int>(location.y - (m_transform.h / 2));


	//int error = SDL_RenderCopyF(pRenderer, m_pSpriteSheet, NULL, &location);
	//if (error)
	//{
	//	std::cout << "SDL_RenderCopyF. Error: " << SDL_GetError() << std::endl;
	//}
	int error = SDL_RenderCopyF(pRenderer, m_pSpriteSheet, &m_sourceTransform, &transform);
	if (error)
	{
		//auto& lua = EngineSingleton::Get()->GetLuaState();
		//lua.DoString("SDL_RenderCopyF. Error: ");
		//lua.DoString(SDL_GetError());
	}
}