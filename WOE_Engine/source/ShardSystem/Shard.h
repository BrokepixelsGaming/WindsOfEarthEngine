#pragma once

#include "EntitySystem/EntityManager.h"
#include "..\3rdParty\include\SDL\SDL.h"

class Shard
{
	bool m_active;

	EntityManager* m_entityManager;
	
public:

	Shard();
	bool IsActive() { return m_active; }
	void UpdateShard(double deltaTime);
	void Render(SDL_Renderer* pRenderer);
	bool LoadShard();
};