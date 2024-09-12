#pragma once

#include <vector>
#include <utility>

#include "ComponentSystem/Collision.h"

class CollisionReferee
{
private:
	// All currently active colliders.
	std::vector<Collision*> m_activeColliders;

public:
	CollisionReferee() = default;
	~CollisionReferee() = default;

	// Register a new collider.
	void AddActiveCollider(Collision* pCollider);

	// Unregister a collider.
	void RemoveActiveCollider(Collision* pCollider);

	// Finds all colliders that overlap and notify them of any collision.
	// Returns wheter or not a collision occurred.
	void CheckForCollisionAndNotify(Collision* pColliderToCheck);
};