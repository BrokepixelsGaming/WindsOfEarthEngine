#pragma once
#include "ComponentSystem\Component.h"
#include "EntitySystem/Entity.h"
#include "tools/Utilities/XorShift.h"
#include "tools/Utilities/Vector.h"
#include <stack>

class LSystemComponent :
    public Component
{
protected:
	StringId m_type = "LSystemComponent";
	Entity* m_pOwner;
	XorShift m_rng;

    std::string m_state = "";
    Vector2<float> m_direction = m_defaultDirection;
    Vector2<float> m_defaultDirection;
    Vector2<float> m_defaultPosition;
    Vector2<float> m_position = m_defaultPosition;
    std::stack<Vector2<float>> m_positionBuffer;

public:
	LSystemComponent(Entity* owner);
    void Update([[maybe_unused]] double deltaTimeMs) override;
    void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset);

private:
    void RunDragonCurveRules();
    void RunStarmapRules();
    std::string AngleVariationForStarmapRules();
    std::string StarColorPicker();
    void DrawStarBurst(SDL_Renderer* pRenderer, Vec2 cameraOffset);
    void DrawStraightLine(SDL_Renderer* pRenderer, Vec2 cameraOffset);
};