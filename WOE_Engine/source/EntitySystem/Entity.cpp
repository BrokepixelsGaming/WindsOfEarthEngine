#include "Entity.h"

#include "ComponentSystem/MoveComponent.h"
#include "EngineInterface/EngineSingleton.h"

Entity::Entity(Vec2 location)
    : m_id(s_nextId)
    , m_components()
    , m_currentLocation(location)
{
    ++s_nextId;
}

Entity::~Entity()
{
    for (auto& component : m_components)
    {
        if(component != nullptr)
            BLEACH_DELETE(component);
    }

    auto evtMgr = EngineSingleton::Get()->GetEventManager();
    if (evtMgr != nullptr)
    {
        evtMgr->RemoveListener(this);
    }
}

bool Entity::ProcessEvent(Event& evt)
{
    for (auto& component : m_components)
    {
        component->ProcessEvent(evt);
    }
    return false;
}

void Entity::Update(double deltaTime)
{
    for (auto& component : m_components)
    {
        component->Update(deltaTime);
    }
}

void Entity::Render( SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
    for (auto& component : m_components)
    {
        component->Render(pRenderer, cameraOffset);
    }
}


void Entity::AddComponent(Component* component)
{
    m_components.push_back(component);
}

void Entity::UpdateLocation(Vec2 deltaLocation)
{
    m_currentLocation.y += deltaLocation.y;
    m_currentLocation.x += deltaLocation.x;
}

void Entity::SetLocation(Vec2 location)
{
    m_currentLocation.y = location.y;
    m_currentLocation.x = location.x;
}
