#pragma once
#include "ComponentSystem/Component.h"
#include "OffsetImage.h"

class StatBarComponent : public Component
{
private:
	OffsetImage* m_backBarImage;
	OffsetImage* m_frontBarImage;
public:
	StatBarComponent(Entity* pOwner, SDL_Renderer* pRenderer, const char* path1, const char* path2, float width1, float height1, float width2, float height2, Vec2 Offset1, Vec2 Offset2);
	~StatBarComponent();

	void Render([[maybe_unused]] SDL_Renderer* pRenderer, Vec2 cameraOffset) override;
};