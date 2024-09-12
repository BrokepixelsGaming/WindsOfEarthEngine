#pragma once

#include <array>

#include "World.h"
#include "config\Settings.h"


///////////////////////////////////////////////////////////////
// A World will be owned by the engine this can be multiple Games or "worlds"
// This will be World -> Shard -> Entity
// A World can own multiple shards
// A Shard can own multiple Entities.
///////////////////////////////////////////////////////////////
class ShardManager;

class WorldManager
{
	// some container of Worlds
	std::array<World, kNumberOfWorlds> m_worlds;
	World* m_currentWorld = nullptr;

public:
	void UpdateWorlds(double deltatime);
	bool LoadWorldsData();
	void DisplayActiveWorld(SDL_Renderer* pRenderer);
	void SetCurrentWorld(int index);
	World* GetCurrentWorld() { return m_currentWorld; };
};

