#include "StatBarComponent.h"
#include "ComponentSystem/Image.h"
//#include "OffsetImage.h"

StatBarComponent::StatBarComponent(Entity* pOwner, SDL_Renderer* pRenderer, const char* path1, const char* path2, float width1, float height1,[[maybe_unused]] float width2,[[maybe_unused]] float height2, Vec2 Offset1, Vec2 Offset2)
	: m_backBarImage(BLEACH_NEW(OffsetImage(pOwner, path1, pRenderer, width1, height1, Offset1)))
	, m_frontBarImage(BLEACH_NEW(OffsetImage(pOwner, path2, pRenderer, (width1 - (width1 /10)), (height1 - (height1 / 10)), Offset2)))
{
}

StatBarComponent::~StatBarComponent()
{
	BLEACH_DELETE(m_backBarImage);
	BLEACH_DELETE(m_frontBarImage);
}

void StatBarComponent::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	if (m_backBarImage != nullptr)
	{
		m_backBarImage->Render(pRenderer, cameraOffset);
	}
	if (m_frontBarImage != nullptr)
	{
		m_frontBarImage->Render(pRenderer, cameraOffset);
	}
}
