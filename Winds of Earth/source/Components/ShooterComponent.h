#pragma once
#include "ComponentSystem\Component.h"
#include "EntitySystem/Entity.h"
#include "EngineInterface/EngineSingleton.h"

class ShooterComponent :
    public Component
{
protected:
	StringId m_type = "ShooterComponent";
	StringId m_round = "Bullet";
	Entity* m_pOwner;
	Entity* m_pTarget;
	EntityID m_targetID;
	int m_range;
	int m_damage;
	const double m_fireRate;
	const float m_projectileSpeed;
	double m_delay;
	bool m_fire;
public:
	ShooterComponent(Entity* pOwner, int range, int damage, double fireRate, float projSpeed)
		: m_pOwner(pOwner)
		, m_pTarget(nullptr)
		, m_range(range)
		, m_damage(damage)
		, m_fireRate(fireRate)
		, m_delay(0)
		, m_projectileSpeed(projSpeed)
		, m_fire(false)
		, m_targetID(0)
	{}
	ShooterComponent(Entity* pOwner, int range, int damage, double fireRate, float projSpeed, const char* round)
		: m_pOwner(pOwner)
		, m_pTarget(nullptr)
		, m_range(range)
		, m_damage(damage)
		, m_fireRate(fireRate)
		, m_delay(0)
		, m_projectileSpeed(projSpeed)
		, m_fire(false)
		, m_round(round)
		, m_targetID(0)
	{}
	void Update([[maybe_unused]] double deltaTime) override;
	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	void SetTarget(Entity* target) { m_pTarget = target; m_targetID = target->GetID(); }
	bool HasTarget() { return (m_pTarget != nullptr); }
	void ClearTarget() { m_pTarget = nullptr; m_targetID = 0; }
	StringId GetType() override { return m_type; }
	int GetRange() { return m_range; }
};

//tempDistance = (std::powf((i), 2) + std::powf((j), 2));