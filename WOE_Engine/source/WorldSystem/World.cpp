#include "World.h"
#include "tools/BleachNew.h"

#include <assert.h>

World::World()
	: m_active(true) // Default is true for now. This is expected to be switched to a default of false. 
	, m_pShardManager(BLEACH_NEW(ShardManager))
	, m_pWorldTileMap(nullptr)
	, m_pEntityManager(BLEACH_NEW(EntityManager))
	, m_backgroundMusic(nullptr)
	, m_CollisionReferee(BLEACH_NEW(CollisionReferee))
{
}

World::~World()
{
	// We will need to ensure correct Deletion of all shards and everything within the shards. 	

	BLEACH_DELETE(m_pShardManager);
	BLEACH_DELETE(m_pEntityManager);
	BLEACH_DELETE(m_CollisionReferee);
}

void World::UpdateWorld(double deltaTime)
{
	// Update all object within the world
	//m_pShardManager->UpdateAllShards(deltaTime); correct code just currently elimintating shards. 
	m_pEntityManager->UpdateEntities(deltaTime);

}

bool World::Init(const char* file)
{
	return m_pWorldTileMap->Init(file);
}

bool World::LoadShardData([[maybe_unused]]const tinyxml2::XMLElement* pEntites)
{
	return true;
}

bool World::LoadEntityData(const tinyxml2::XMLElement* pEntites)
{
	return m_pEntityManager->LoadEntities(pEntites);
}

void World::SetMap(TileMap* map)
{
	if (map)
	{
		m_pWorldTileMap = map;
	}
}

StringId World::GetTileTypeByLocation(float x, float y)
{
	return m_pWorldTileMap->GetTileTypeByLocation(x, y);
}

void World::SetTileTypeByLocation(float x, float y, TileType type)
{
	m_pWorldTileMap->SetTileTypeByLocation(x, y, type);
}

void World::SetTileTypeByLocation(int x, int y, TileType type)
{
	m_pWorldTileMap->SetTileTypeByLocation(static_cast<float>(x), static_cast<float>(y), type);
}

StringId World::GetTileOverlayTypeByLocation(float x, float y)
{
	return m_pWorldTileMap->GetTileOverlayTypeByLocation(x, y);
}

void World::SetTileAvailabilityByLocation(float x, float y, bool isAvailable)
{
	m_pWorldTileMap->SetTileAvailabilityByLocation(x, y, isAvailable);
}

void World::SetAllTilesToType(TileType type)
{
	m_pWorldTileMap->SetAllTileBaseLayerToType(type);
}

bool World::GetTileAvailabilityByLocation(float x, float y)
{
	return m_pWorldTileMap->GetTileAvailabilityByLocation(x, y);
}

bool World::TileHasOverlay(float x, float y)
{
	return m_pWorldTileMap->TileHaveOverlay(x, y);
}

void World::CollectLocation(const Vec2& location)
{
	m_pWorldTileMap->ClearOverlay(location);
}

void World::Render(SDL_Renderer* pRenderer)
{
	m_pWorldTileMap->Render(pRenderer);

	m_pEntityManager->Render(pRenderer);
}
