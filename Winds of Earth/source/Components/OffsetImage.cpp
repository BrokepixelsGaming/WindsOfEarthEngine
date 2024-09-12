#include "OffsetImage.h"
#include <iostream>
#include "SDL/SDL_image.h"

OffsetImage::OffsetImage(Entity* pOwner, const char* path, SDL_Renderer* pRenderer, float width, float height, Vec2 Offset)
	: Image(pOwner,path,pRenderer, width, height)
	, m_offsetAmount(Offset)
{
}

OffsetImage::~OffsetImage()
{
}

void OffsetImage::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	// Get the location of the owner. 
	const auto& location = m_pOwner->GetLocation();

	// We will update our internal rect at this point.
	m_transform.x = location.x - (m_transform.w / 2) + m_offsetAmount.x;
	m_transform.y = location.y - (m_transform.h / 2) + m_offsetAmount.y;
	m_transform.x -= cameraOffset.x;
	m_transform.y -= cameraOffset.y;

	int error = SDL_RenderCopyF(pRenderer, m_pTexture, NULL, &m_transform);
	if (error)
	{
		std::cout << "SDL_RenderCopyF. Error: " << SDL_GetError() << std::endl;
	}
}
