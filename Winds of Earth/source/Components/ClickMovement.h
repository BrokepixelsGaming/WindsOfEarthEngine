#pragma once
#include "ComponentSystem\Component.h"
#include "EntitySystem/Entity.h"
#include "config/Settings.h"
#include "TileMapSystem/PathPlan.h"

class ClickMovement :
    public Component
{
protected:
	StringId m_type = "ClickMovement";
	Entity* m_pOwner;

private:
	Vec2 m_targetLocation;

	static constexpr float kSpeed = 100.0f;


	//Vec2 m_position;

	Plan m_path;

	static constexpr int kMaxX = 2000;
	static constexpr int kLowX = 0;
	static constexpr int kMaxY = 2000;
	static constexpr int kLowY = 0;
	static constexpr int kFenceYLimit = 0;
	static constexpr float kPathingNodeTolerance = 0.5f;
	bool m_flyer;

public:
	ClickMovement(Entity* pOwner);
	ClickMovement(Entity* pOwner, bool flyer);
	~ClickMovement() = default;
	void Update([[maybe_unused]] double deltaTime) override;
	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	StringId GetType() override { return m_type; }
	void StopMovement() { m_targetLocation = m_pOwner->GetLocation(); }

	void StepMovement(double deltaTimeMs);

	// accessors
	const Vec2& GetPos() const { return m_pOwner->GetLocation(); };
};

