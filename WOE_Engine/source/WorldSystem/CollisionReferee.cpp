#include "CollisionReferee.h"
#include "EntitySystem/Entity.h"
#include "../../Winds of Earth/source/Components/ProjectileComponent.h"
#include "../../Winds of Earth/source/Components/TeamComponent.h"

void CollisionReferee::AddActiveCollider(Collision* pCollider)
{
	// Make sure it's not already present.
	if (std::find(m_activeColliders.begin(), m_activeColliders.end(), pCollider) == m_activeColliders.end())
	{
		// If it's not already present, add it to the vector.
		m_activeColliders.push_back(pCollider);
	}
}

void CollisionReferee::RemoveActiveCollider(Collision* pCollider)
{
	// Find the collider
	auto location = std::find(m_activeColliders.begin(), m_activeColliders.end(), pCollider);

	// If it's found, erase it from the vector.
	if (location != m_activeColliders.end())
	{
		m_activeColliders.erase(location);
	}
}

void CollisionReferee::CheckForCollisionAndNotify(Collision* pColliderToCheck)
{

	std::pair<bool, Entity*> collideAndWhom;
	// Allocate a bool for whether any collisions have occurred.
	collideAndWhom.first = false;
	collideAndWhom.second = nullptr;

	// Get pColliderToCheck's transform.
	SDL_FRect colliderTransform = pColliderToCheck->GetTransform();

	// Calaculate bounds.
	float left = colliderTransform.x;
	float right = left + colliderTransform.w;
	float top = colliderTransform.y;
	float bottom = top + colliderTransform.h;

	// Check against every active collider.
	for (Collision* pOtherCollider : m_activeColliders)
	{
		// Make sure we don't check a collider vs itself.
		if (pColliderToCheck == pOtherCollider)
		{
			continue;
		}
		// Here We check if we are a projectile. TODO: FIND A WAY TO PULL THIS OUT LATER. 
		if (pColliderToCheck->GetOwner()->GetComponent<ProjectileComponent>("ProjectileComponent"))
		{
			if(pOtherCollider->GetOwner()->GetComponent<ProjectileComponent>("ProjectileComponent"))
				continue;
		}

		//// Here We check if we are a projectile. TODO: FIND A WAY TO PULL THIS OUT LATER. 
		//if (pColliderToCheck->GetOwner()->GetComponent<TeamComponent>("TeamComponent")->GetTeamNumber() == pOtherCollider->GetOwner()->GetComponent<TeamComponent>("TeamComponent")->GetTeamNumber())
		//{
		//	continue;
		//}

		// Get other collider's transform.
		SDL_FRect otherColliderTransform = pOtherCollider->GetTransform();

		// Calculate the bounds of the other collider.
		float otherLeft = otherColliderTransform.x;
		float otherRight = otherLeft + otherColliderTransform.w;
		float otherTop = otherColliderTransform.y;
		float otherBottom = otherTop + otherColliderTransform.h;

		// Check if they overlap horizontally.
		bool xOverlap = left < otherRight&& right > otherLeft;

		// Check if they overlap vertically.
		bool yOverlap = top < otherBottom&& bottom > otherTop;

		// If both x & y are true, they collide.
		if (xOverlap && yOverlap)
		{
			// A collision has occurred.
			collideAndWhom.first = true;
			collideAndWhom.second = pOtherCollider->GetOwner();

			// Notify both colliders' owners of the collision.
			pColliderToCheck->OnCollision(pOtherCollider); // notify object A
			pOtherCollider->OnCollision(pColliderToCheck); // notify object B
		}
	}
}
