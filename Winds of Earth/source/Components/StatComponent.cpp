#include "StatComponent.h"

StatComponent::StatComponent(Entity* pOwner,int currentHealth = 0, int maxHealth = 0)
	: m_pOwner(pOwner)
	, m_currentHealth(currentHealth)
	, m_maxHealth(maxHealth)
	, m_damage(0)
	, m_armor(0)
	, m_speed(0)
{
}

StatComponent::~StatComponent()
{
}
