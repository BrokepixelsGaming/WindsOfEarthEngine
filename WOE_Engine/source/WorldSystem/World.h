#pragma once

#include <vector>

#include "ShardSystem/ShardManager.h"
#include "TileMapSystem/TileMap.h"
#include "ResourceSystem/Resource.h"
#include "ComponentSystem/Audio.h"
#include "TileMapSystem/TileTypes.h"
#include "CollisionReferee.h"
#include "../tools/Utilities/StringId.h"
#include "tools/Utilities/Vector.h"


class World
{
private:
	bool m_active; // if the world is active or not. 

	// shard Manager?
	ShardManager* m_pShardManager;
	// or container of shards?
	TileMap* m_pWorldTileMap;
	EntityManager* m_pEntityManager;
	Audio* m_backgroundMusic;
	CollisionReferee* m_CollisionReferee;

	bool m_powerUp = false;
	double m_powerUpTimer = 0;

public:
	World();
	~World();

	bool IsActive() { return m_active; } // returns the if the world is active or not. 
	void UpdateWorld(double deltaTime);
	CollisionReferee* GetCollisionRef() { return m_CollisionReferee; }
	EntityManager* GetEntityManager() { return m_pEntityManager; }
	bool Init(const char* file);
	bool LoadShardData(const tinyxml2::XMLElement* pShards);
	bool LoadEntityData(const tinyxml2::XMLElement* pEntites);
	void SetMap(TileMap* map);
	void SetMusic(Audio* music) { m_backgroundMusic = music; }
	void SetInactive() { m_active = false; }
	void SetActive() { m_active = true; }
	void PlayMusic() { m_backgroundMusic->PlaySound(); }
	Plan GeneratePathToBestTarget(const Vec2& startPos, const Vec2& endPos) const { return m_pWorldTileMap->GeneratePathToBestTarget(startPos, endPos); }
	Vec2 GetRandomWalkableTilePos() const { return m_pWorldTileMap->GetRandomWalkableTilePos(); }

	Vec2 GetLocationOfRandomTileByType(TileType type) const { return m_pWorldTileMap->GetLocationOfRandomTileByType(type); };
	StringId GetTileTypeByLocation(float x, float y);

	///////////////////////////////////////////////////////////////
	//	 SET TILES
	///////////////////////////////////////////////////////////////
	void SetTileTypeByLocation(float x, float y, TileType type);
	void SetTileTypeByLocation(int x, int y, TileType type);
	StringId GetTileOverlayTypeByLocation(float x, float y);
	void SetTileAvailabilityByLocation(float x, float y, bool isAvailable);
	void SetAllTilesToType(TileType type);

	///////////////////////////////////////////////////////////////
	//   GET TILES
	///////////////////////////////////////////////////////////////
	bool GetTileAvailabilityByLocation(float x, float y);
	int GetTileIndexByLocation(float x, float y) { return m_pWorldTileMap->GetTileIndexByLocation(x, y); }
	int GetTileIndexByLocation(int x, int y) { return m_pWorldTileMap->GetTileIndexByLocation(static_cast<float>(x), static_cast<float>(y)); }


	bool TileHasOverlay(float x, float y);
	void CollectLocation(const Vec2& location);
	bool CollectableRemaining() { return m_pWorldTileMap->CollectablesLeft(); }
	bool PowerUp() { return m_powerUp; }
	void SetPowerUp(bool input) { m_powerUp = input; }
	void SetTimer(double deltaTime) { m_powerUpTimer += deltaTime; }
	bool TimeUp() { return (m_powerUpTimer < 0); }
	void Render(SDL_Renderer* pRenderer);
	void ResetMap() { m_pWorldTileMap->ResetMap(); }
	std::vector<std::pair<StringId, StringId>> GetWorldMapCopy() { return m_pWorldTileMap->GetMapState(); }

	Vec2 GetTileMapWidthAndHeight() { return m_pWorldTileMap->GetTileMapWidthAndHeight(); };

	// Snap to tile point
	int SnapX(int xPos) { return m_pWorldTileMap->SnapX(xPos); }
	int SnapY(int yPos) { return m_pWorldTileMap->SnapY(yPos); }
};