#pragma once
#include "ComponentSystem/Image.h"

class OffsetImage : public Image
{
private:
	Vec2 m_offsetAmount;
public:
	OffsetImage(Entity* pOwner, const char* path, SDL_Renderer* pRenderer, float width, float height,Vec2 Offset);
	~OffsetImage();

	void Render([[maybe_unused]] SDL_Renderer* pRenderer, Vec2 cameraOffset) override;
};
