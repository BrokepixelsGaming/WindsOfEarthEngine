
#include "Image.h"
#include <iostream>
#include "SDL/SDL_image.h"

Image::Image(Entity* pOwner, const char* path, SDL_Renderer* pRenderer, float width, float height )
	: m_transform{pOwner->GetLocation().x, pOwner->GetLocation().y, width, height}
	, m_pOwner(pOwner)
{
	// Attempt to load the image to a surface.
	SDL_Surface* pSpriteSheetSurface = IMG_Load(path);

	// Error checking 
	if (pSpriteSheetSurface == nullptr)
	{
		std::cout << "failed to Get Surface. Error:" << SDL_GetError();
		return;
	}

	// Create the texture.
	m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pSpriteSheetSurface);

	// Error check.
	if (m_pTexture == nullptr)
	{
		std::cout << "failed to Get Texture. Error:" << SDL_GetError();
		return;
	}

	// Free the surface. 
	SDL_FreeSurface(pSpriteSheetSurface);

}

Image::~Image()
{
	// Destroy the texture. 
	SDL_DestroyTexture(m_pTexture);
}

void Image::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	if (m_activeRendering) 
	{
		// Get the location of the owner. 
		const auto& location = m_pOwner->GetLocation();

		// We will update our internal rect at this point.
		m_transform.x = location.x - (m_transform.w / 2);
		m_transform.y = location.y - (m_transform.h / 2);
		m_transform.x -= cameraOffset.x;
		m_transform.y -= cameraOffset.y;

		int error = SDL_RenderCopyF(pRenderer, m_pTexture, NULL, &m_transform);
		if (error)
		{
			std::cout << "SDL_RenderCopyF. Error: " << SDL_GetError() << std::endl;
		}
	}
}