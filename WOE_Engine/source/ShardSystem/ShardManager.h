#pragma once

#include <array>

#include "Shard.h"
#include "config/Settings.h"
#include "..\3rdParty\include\SDL\SDL.h"



///////////////////////////////////////////////////////////////
// A shard is owned by a World and is one of many. This can also be thought of as a "area" "state" or a location.
///////////////////////////////////////////////////////////////
class ShardManager
{
	std::array<Shard, kMaxShardsPerWorld> m_shards;

public:

	void UpdateAllShards(double deltaTime);
	void RenderShards(SDL_Renderer* pRenderer);
	bool LoadShardData();
};

