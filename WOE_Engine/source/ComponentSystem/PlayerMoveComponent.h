#pragma once
#include "ComponentSystem/MoveComponent.h"
#include "ComponentSystem/Component.h"
#include "EntitySystem/Entity.h"
#include "config/Settings.h"
#include "TileMapSystem/PathPlan.h"
#include "tools/Utilities/SeededGenerator.h"
#include "Image.h"

class PlayerMoveComponent :
    public MoveComponent
{


protected:
	StringId m_type = "PlayerMovement";
	int m_lastKeystroke = 0;
	int m_lastValidKeystroke = 0;
	static constexpr float kPathingNodeTolerance = 0.5f;
	Plan m_path;
	Image* m_pImage;
	Vec2 m_changeInMove;

	bool m_leftMove = false;
	bool m_rightMove = true;
	bool m_upMove = false;
	bool m_downMove = false;

public:

	PlayerMoveComponent(Entity* owner);
	~PlayerMoveComponent();
	bool ProcessEvent([[maybe_unused]] Event& evt) override;
	void Update(double deltaTime) override;
	void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset) override;

	// accessors
	void SetPath(const Plan& path);
	const Vec2& GetPos() const { return m_pOwner->GetLocation(); };

private:
	void StepMovement(double deltaTimeMs);
};

