#pragma once
#include "MoveComponent.h"

#include "TileMapSystem/PathPlan.h"

class EnemyMoveComponent :
    public MoveComponent
{
protected:
    StringId m_type = "EnemyMovement";
public:

    static constexpr float kSpeed = 100.0f;
    static constexpr float kPathingNodeTolerance = 2.f;

    //Vec2 m_position;

    Plan m_path;

public:
    EnemyMoveComponent(Entity* owner);
    // update functions
    void Update(double deltaTimeMs);
    bool ProcessEvent([[maybe_unused]] Event& evt) override;

    // accessors
    void SetPath(const Plan& path);
    const Vec2& GetPos() const { return m_pOwner->GetLocation(); };

private:
    void StepMovement(double deltaTimeMs);
};

