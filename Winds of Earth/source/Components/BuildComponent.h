#pragma once
#include "ComponentSystem/Component.h"
#include "EntitySystem/Entity.h"

class BuildComponent : public Component
{
protected:
	StringId m_type = "Build";
private:
	Entity* m_pOwner;
	bool m_canTryToBuild;
public:
	BuildComponent(Entity* owner);
	~BuildComponent();

	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	void SetTryToBuild(bool state);
	virtual StringId GetType() override { return m_type; }
private:
	bool HitTest(int x, int y);
};
