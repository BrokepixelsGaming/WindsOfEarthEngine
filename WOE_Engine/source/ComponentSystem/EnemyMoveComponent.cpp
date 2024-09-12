#include "EnemyMoveComponent.h"
#include "EngineInterface/EngineSingleton.h"
#include "tools/Utilities/Vector.h"

EnemyMoveComponent::EnemyMoveComponent(Entity* owner)
    : MoveComponent(owner)
{
}

void EnemyMoveComponent::Update(double deltaTimeMs)
{
    if (!m_path.empty())
    {
        StepMovement(deltaTimeMs);
    }
    else
    {
        // Get a new path. 
        auto path = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GeneratePathToBestTarget(m_pOwner->GetLocation(), EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetRandomWalkableTilePos());
        SetPath(path);
    }
}

bool EnemyMoveComponent::ProcessEvent(Event& evt)
{
    std::string pac = "PacMan";
    if (evt.GetTargetEntityID() == m_pOwner->GetID())
    {
        if (evt.GetEventType() == Event::EventType::Collision)
        {
            auto& man = evt.GetCommandString();

            if (pac == man)
            {
                if (EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->PowerUp())
                {
                    //std::cout << "You hit ghost and the Ghost Died.\n";
                    //EngineSingleton::Get()->GetEventManager()->QueueEvent(Event(Event::EventType::DestroyEntity, m_pOwner->GetID(), "Destroy"));
                    m_pOwner->SetLocation(Vec2{ 250,250 });
                    m_path.clear();
                }
            }
        }
    }
    return false;
}

void EnemyMoveComponent::SetPath(const Plan& path)
{
    m_path.clear();
    m_path = path;
}

void EnemyMoveComponent::StepMovement(double deltaTimeMs)
{
    // step the game object towards their goal
    Vec2 direction = m_path.front() - GetPos();
    direction.Normalize();
    Vec2 deltaPosition = direction * kDefaultSpeed * (float)deltaTimeMs;
    m_pOwner->UpdateLocation(deltaPosition);

    // check to see if we've arrived
    Vec2 delta = m_path.front() - GetPos();
    if (delta.Length() <= kPathingNodeTolerance)
        m_path.pop_front();
}
