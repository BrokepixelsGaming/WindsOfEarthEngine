#include "TileDef.h"
#include "Tile.h"
#include "../3rdParty/include/SDL/SDL_image.h"
#include "../EngineInterface/EngineSingleton.h"
#include "../tools/BleachNew.h"
#include "../RenderSystem/Camera.h"

//------------------------------------------------------------------------------------------------------------------
// Default constructor.
//------------------------------------------------------------------------------------------------------------------
TileDef::TileDef(const char* imageSource, int x, int y, int w, int h, StringId type, float weight)
{
	// TODO(Josh): replace this with some texture resource or abstraction from SDL

	// Create a surface
	SDL_Surface* pSurface = IMG_Load(imageSource);

	// Get the renderer
	SDL_Renderer* pRenderer = EngineSingleton::Get()->GetWindow()->GetRenderer();

	// Create a texture
	m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);

	// Free the surface
	SDL_FreeSurface(pSurface);

	// Set the transform
	m_sourceTransform.x = x;
	m_sourceTransform.y = y;
	m_sourceTransform.w = w;
	m_sourceTransform.h = h;

	// Set the type
	m_type = type;

	// Set the weight
	m_weight = weight;

}

//------------------------------------------------------------------------------------------------------------------
// Creates a tile using this definition.
//------------------------------------------------------------------------------------------------------------------
Tile* TileDef::CreateTile(int x, int y)
{
	// TODO(Josh): Get rid of nullptr and actually instantiate a tile
	// New up a tile passing in this tile def as arguments.
	Tile* pTile = BLEACH_NEW(Tile(x * m_sourceTransform.w, y * m_sourceTransform.h, this));
	
	// Return the tile
	return pTile;
}

void TileDef::Render(int x, int y)
{
	// Get the camera
	Camera& camera = EngineSingleton::Get()->GetCamera();

	// Render if it is inside the camera
	if (camera.IsInCamera(x, y, m_sourceTransform.w, m_sourceTransform.h))
	{
		SDL_Renderer* pRenderer = EngineSingleton::Get()->GetWindow()->GetRenderer();
		SDL_Rect transform{ x - camera.GetOffsetX(), y - camera.GetOffsetY(), m_sourceTransform.w, m_sourceTransform.h};
		SDL_RenderCopy(pRenderer, m_pTexture, &m_sourceTransform, &transform);
	}
}

StringId TileDef::GetType()
{
	return m_type;
}
