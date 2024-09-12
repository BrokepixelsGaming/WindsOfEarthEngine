#pragma once
#include "ComponentSystem/Component.h"
#include "EntitySystem/Entity.h"

class StatComponent : public Component
{
protected:
	StringId m_type = "Stat";
private:
	Entity* m_pOwner;

	int m_currentHealth;
	int m_maxHealth;

	int m_speed;

	int m_damage;
	int m_armor;
public:
	StatComponent(Entity* pOwner, int currentHealth, int maxHealth);
	~StatComponent();

	int GetCurrentHealth() { return m_currentHealth; }
	int GetMaxHealth() { return m_maxHealth; }

	void SetCurrentHealth(int currentHealth) { m_currentHealth = currentHealth; }
	void SetMaxHealth(int maxHealth) { m_maxHealth = maxHealth; }
};
