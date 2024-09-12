#pragma once
#include "ComponentSystem/Component.h"
#include "EntitySystem/Entity.h"

class SelectorComponent :
    public Component
{
protected:
	StringId m_type = "SelectorComponent";
private:
	Entity* m_pOwner;
	float m_ownerWidth;
	float m_ownerHeight;
	bool m_selected;
	static constexpr float kSpacing = 2;
public:
	SelectorComponent(Entity* owner, float ownerHeight, float ownerWidth);
	~SelectorComponent();

	void Render(SDL_Renderer* pRenderer, Vec2 cameraOffset) override;
	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	bool IsSelected() { return m_selected; }
	virtual StringId GetType() override { return m_type; }
private:
	bool HitTest(int x, int y);
};

