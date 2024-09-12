#include "Collision.h"
#include "EntitySystem/Entity.h"

#include <vector>

#include "WorldSystem/CollisionReferee.h"
#include "EngineInterface/EngineSingleton.h"
#include "EventSystem/Event.h"


Collision::Collision(Entity* pOwner, float height, float width, CollisionReferee* pReferee, const char* message)
	: m_pOwner(pOwner)
	, m_transform { pOwner->GetLocation().x, pOwner->GetLocation().y, height, width }
	, m_pReferee(pReferee)
	, m_message(message)
{
	if (pReferee != nullptr)
	{
		pReferee->AddActiveCollider(this);
	}
}

Collision::~Collision()
{
	if (m_pReferee != nullptr)
	{
		m_pReferee->RemoveActiveCollider(this);
	}
	m_pOwner = nullptr;
}

void Collision::Update([[maybe_unused]] double deltaTime)
{
	m_transform.x = m_pOwner->GetLocation().x;
	m_transform.y = m_pOwner->GetLocation().y;
	EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetCollisionRef()->CheckForCollisionAndNotify(this);
}

///////////////////////////////////////////////////////////////
// Updates Position.
///////////////////////////////////////////////////////////////
void Collision::SetPosition(SDL_FRect newPosition)
{
	m_transform.x = static_cast<float>(newPosition.x);
	m_transform.y = static_cast<float>(newPosition.y);
}

void Collision::OnCollision(Collision* potherCollider)
{
	auto& message = m_message;
	auto ID = potherCollider->GetOwner()->GetID();
	EngineSingleton::Get()->GetEventManager()->QueueEvent(Event(Event::EventType::Collision, ID , message.c_str()));
}

bool Collision::HitCheck(float x, float y)
{
	return(x >= m_pOwner->GetLocation().x - (m_transform.w / 2) && x <= m_pOwner->GetLocation().x + (m_transform.w / 2) &&
		y >= m_pOwner->GetLocation().y - (m_transform.h / 2) && y <= m_pOwner->GetLocation().y + (m_transform.h / 2));
}
