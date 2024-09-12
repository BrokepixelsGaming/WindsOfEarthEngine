#pragma once
#include "ComponentSystem\Component.h"
#include "EntitySystem/Entity.h"

class ProjectileComponent :
    public Component
{
protected:
    StringId m_type = "ProjectileComponent";
    Entity* m_pOwner;
    Vec2 m_direction;
    float m_speed;
    double m_lifetime = 3;
public:
    ProjectileComponent(Entity* pOwner)
        : m_pOwner(pOwner)
        , m_speed (1)
    {}

    void SetDirection(Vec2 direction) { m_direction = direction; }
    void SetSpeed(float speed) { m_speed = speed; }
    void Update([[maybe_unused]] double deltaTime) override;
    bool ProcessEvent([[maybe_unused]] Event& evt) override;
    StringId GetType() override { return m_type; }
    
private:
    void StepMovement(double deltaTime);
};