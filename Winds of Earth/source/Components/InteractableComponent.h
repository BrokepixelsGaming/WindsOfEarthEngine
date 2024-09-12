#pragma once
#include "ComponentSystem/Component.h"
#include "EntitySystem/Entity.h"
#include "tools/Utilities/Vector.h"

class InteractableComponent : public Component
{
private:
	bool m_spawnDelay;
protected:
	Entity* m_pOwner;
	StringId m_type = "Interact";
	Vec2 m_size;

public:
	InteractableComponent(Entity* owner, Vec2 size);
	~InteractableComponent();

	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	void Update([[maybe_unused]] double deltaTime) override;

private:
	virtual bool HitTest(int x, int y);
};
