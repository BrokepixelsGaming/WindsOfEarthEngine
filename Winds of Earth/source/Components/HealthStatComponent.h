#pragma once
#include "../../WOE_Engine/source/ComponentSystem/Component.h"
#include "../../WOE_Engine/source/ComponentSystem/Animation.h"
#include "../../WOE_Engine/source/EntitySystem/Entity.h"

class HealthStatComponent :
    public Component
{
protected:
    StringId m_type = "HealthStatComponent";
private:
    // Name of object
    StringId m_healType = "None";

    // Transformation of the object.
    SDL_FRect m_transform;

    // Animation component
    Animation m_animation;

    // Pointer to object to display health of. 
    Entity* m_pOwner;

    // Value of each displayed heart. 
    int m_heartValue;
    int m_numHearts;
    int m_maxHealth;
    int m_currentHealth;

public:
    HealthStatComponent(Entity* pOwner, SDL_Renderer* pRenderer, SDL_FRect transform, int heartValue, int numHearts, int maxHealth );
    HealthStatComponent(Entity* pOwner, SDL_Renderer* pRenderer, SDL_FRect transform, int heartValue, int numHearts, int maxHealth, const char* healType );
    ~HealthStatComponent() = default;

    StringId GetType() override { return m_type; }
    void UpdateHealth(int amount) { m_currentHealth += amount; }

    // Inherited via GameObject
    virtual void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset) override;
    bool ProcessEvent([[maybe_unused]] Event& evt)override;
    StringId GetHealType() { return m_healType; }
};