#pragma once
#include "Component.h"
#include <SDL/SDL.h>
#include <string>

class Entity;
class CollisionReferee;

class Collision :
	public Component
{
protected:
	StringId m_type = "Collision";
private:
	// Pointer to GameObject that owns this collider.
	Entity* m_pOwner;

	// Position and dimensions.
	SDL_FRect m_transform;

	// Pointer to referee for registering and collision checking.
	CollisionReferee* m_pReferee;

	// Message to sent via event system.
	std::string m_message;

public:
	Collision(Entity* pOwner, float height, float width, CollisionReferee* pReferee, const char* message);
	~Collision();

	// Returns pointer to this collider's owner.
	Entity* GetOwner() { return m_pOwner; }

	void Update([[maybe_unused]] double deltaTime) override;

	// Returns Position and dimensions.	
	SDL_FRect GetTransform() { return m_transform; }

	StringId GetType()override { return m_type; }

	// Updates position.
	void SetPosition(SDL_FRect newPosition);
	void OnCollision(Collision* potherCollider);
	void SetMessage(const char* message) { m_message = message; }
	bool HitCheck(float x, float y);

	const std::string& GetMessage() { return m_message; }
	// Attempts to move by the given amount
	// Returns wheter the collision was successful.
	//std::pair<bool, Entity*> TryMove(SDL_FRect deltaPosition);
};

