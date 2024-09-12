#include "UITextField.h"

UITextField::UITextField(TTF_Font* pFont, const char* pText, SDL_Color color, SDL_FRect position, SDL_Renderer* pRenderer)
	: m_pTexture(nullptr)
	, m_pFont(pFont)
	, m_color(color)
{
	// create the surface.
	SDL_Surface* pTextSurface = TTF_RenderText_Solid(m_pFont, pText, m_color);
	// Error checking.
	// Create the Texture
	m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pTextSurface);

	// Get the dimensions of the text and apply them to m_transform
	m_transform.x = position.x;
	m_transform.y = position.y;

	m_transform.w = position.w;
	m_transform.h = position.h;
	//m_transform.w = pTextSurface->w;
	//m_transform.h = pTextSurface->h;

	SDL_FreeSurface(pTextSurface);
}

UITextField::~UITextField()
{
	SDL_DestroyTexture(m_pTexture);
}

void UITextField::Render(SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset)
{
	SDL_RenderCopyF(pRenderer, m_pTexture, nullptr, &m_transform);
}


void UITextField::UpdateLocation(float x, float y)
{
	m_transform.x += x;
	m_transform.y += y;
}

// Pass Zero or negative to not change the size. 
void UITextField::SetHeightWidth(float h, float w)
{
	if(h > 0)
		m_transform.h = h;
	if(w > 0)
		m_transform.w = w;
}

void UITextField::UpdateText(const char* pText)
{
	SDL_DestroyTexture(m_pTexture);
	SDL_Surface* pTextSurface = TTF_RenderText_Solid(m_pFont, pText, m_color);
	// Error checking.
	// Create the Texture
	m_pTexture = SDL_CreateTextureFromSurface(EngineSingleton::Get()->GetWindow()->GetRenderer(), pTextSurface);
	SDL_FreeSurface(pTextSurface);
}
