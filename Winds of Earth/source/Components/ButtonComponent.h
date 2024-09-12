#pragma once
#include "InteractableComponent.h"

class ButtonComponent : public InteractableComponent
{
private:
	bool m_isHighlighted;

public:
	ButtonComponent(Entity* owner, Vec2 size);
	~ButtonComponent();

	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	void Update([[maybe_unused]] double deltaTime) override;

private:
	virtual bool HitTest(int x, int y) override;
};