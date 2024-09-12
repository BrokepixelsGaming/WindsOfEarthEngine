#pragma once
#include "ComponentSystem\Component.h"
#include "EntitySystem/Entity.h"
#include "EngineInterface/EngineSingleton.h"
#include "TeamComponent.h"
#include "ProjectileComponent.h"
#include "ComponentSystem/Image.h"
#include "../../WOE_Engine/source/ComponentSystem/Collision.h"
#include "SelectorComponent.h"


class MeleeComponent :
    public Component
{
protected:
	StringId m_type = "MeleeComponent";
	StringId m_typeToHeal = "None";
	Entity* m_pOwner;
	Entity* m_pTarget;
	EntityID m_targetID;

	int m_sightRange;
	int m_range;
	int m_damage;
	const double m_fireRate;

	double m_delay;
	bool m_fire;
	bool m_healer;
public:
	MeleeComponent(Entity* pOwner, int range, int damage, double fireRate)
		: m_pOwner(pOwner)
		, m_range(range)
		, m_pTarget(nullptr)
		, m_damage(damage)
		, m_fireRate(fireRate)
		, m_delay(0)
		, m_fire(false)
		, m_healer(false)
		, m_targetID(0)
	{}
	MeleeComponent(Entity* pOwner, int range, int damage, double fireRate, bool healer, const char* typeCanHeal)
		: m_pOwner(pOwner)
		, m_pTarget(nullptr)
		, m_range(range)
		, m_damage(damage)
		, m_fireRate(fireRate)
		, m_delay(0)
		, m_fire(false)
		, m_healer(healer)
		, m_typeToHeal(typeCanHeal)
		, m_targetID(0)
	{}
	void Update([[maybe_unused]] double deltaTime) override;
	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	void SetTarget(Entity* target) { m_pTarget = target; m_targetID = target->GetID();}
	bool HasTarget() { return (m_pTarget != nullptr); }
	void ClearTarget() { m_pTarget = nullptr; m_targetID = 0; }
	StringId GetType() override { return m_type; }
	StringId HealType() { return m_typeToHeal; }
	int GetRange() { return m_range; }
};

