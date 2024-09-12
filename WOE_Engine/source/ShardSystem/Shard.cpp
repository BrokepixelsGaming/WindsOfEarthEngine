#include "Shard.h"

Shard::Shard()
	: m_active(true)  // Default is true for now. This is expected to be switched to a default of false.
	, m_entityManager()
{
}

void Shard::UpdateShard(double deltaTime)
{
	m_entityManager->UpdateEntities(deltaTime);
}

void Shard::Render(SDL_Renderer* pRenderer)
{
	m_entityManager->Render(pRenderer);
}

bool Shard::LoadShard()
{
	return false;//m_entityManager->LoadEntities();
}
