#pragma once
#include "ComponentSystem\Component.h"
#include "EntitySystem/Entity.h"
#include "config/Settings.h"
#include "EngineInterface/EngineSingleton.h"
#include "../Components/TeamComponent.h"

class SightMovement :
    public Component
{
protected:
	StringId m_type = "SightMovement";
	Entity* m_pOwner;
	Entity* m_pTarget;
	EntityID m_targetID;

private:
	Vec2 m_targetLocation;
	static constexpr int kMaxX = 2736;
	static constexpr int kLowX = 25;
	static constexpr int kMaxY = 730;
	static constexpr int kLowY = 25;
	static constexpr int kFenceYLimit = 325;
	static constexpr float kPathingNodeTolerance = 0.5f;
	int m_sightRange;
	bool m_flyer;
	bool m_stopMoving;

public:
	SightMovement(Entity* pOwner, int sightRange);
	SightMovement(Entity* pOwner, int sightRange, bool flyer);
	~SightMovement() = default;
	void Update([[maybe_unused]] double deltaTime) override;
	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	void ClearTarget() { m_pTarget = nullptr; m_targetID = 0; }
	StringId GetType() override { return m_type; }
	void StopMovement();

	void StepMovement(double deltaTimeMs);

	// accessors
	const Vec2& GetPos() const { return m_pOwner->GetLocation(); };
};

