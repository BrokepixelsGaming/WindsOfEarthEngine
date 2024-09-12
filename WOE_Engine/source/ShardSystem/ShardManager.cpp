#include "ShardManager.h"

void ShardManager::UpdateAllShards(double deltaTime)
{
	for (size_t i = 0; i < kMaxShardsPerWorld; ++i)
	{
		if (m_shards[i].IsActive())
		{
			m_shards[i].UpdateShard(deltaTime);
		}
	}
}

void ShardManager::RenderShards(SDL_Renderer* pRenderer)
{
	for (size_t i = 0; i < kMaxShardsPerWorld; ++i)
	{
		if (m_shards[i].IsActive())
		{
			m_shards[i].Render(pRenderer);
		}
	}
}

bool ShardManager::LoadShardData()
{
	return m_shards[0].LoadShard();
}
