#pragma once
#include "Component.h"
#include "EntitySystem/Entity.h"
#include "Audio.h"

class CollectorComponent :
    public Component
{
private:
	Entity* m_pOwner;
protected:
	Audio* m_collectionSFX;
	Audio* m_powerUpMusic;
	int m_count;
	StringId m_type = "Collector";
public:
	CollectorComponent(Entity* owner);
	~CollectorComponent();
	StringId GetType() override { return m_type; }
	int GetCount() { return m_count; }
	void Update([[maybe_unused]] double deltaTime) override;
};