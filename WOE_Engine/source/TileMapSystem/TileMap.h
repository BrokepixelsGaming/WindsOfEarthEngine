// World.h
#pragma once

#include "..\3rdParty\include\SDL\SDL.h"
#include "..\TileMapSystem\PathPlan.h"
#include "ResourceSystem/Resource.h"
#include "..\tools\Utilities\StringId.h"
#include "TileTypes.h"
#include "config/Settings.h"
#include "TileDef.h"
#include "tools/Utilities/Vector.h"

#include <vector>
#include <unordered_map>

class Tile;
class PathPlanNode;
class TilePalatte;

class TileMap : public Resource
{

private:
    // tile grid
    std::vector<Tile*> m_tiles;
    std::vector<int> m_goalIndex;

    int m_worldWidth;
    int m_worldHeight;
    int m_numTiles = m_worldHeight * m_worldWidth;

    // The palatte we use to build the tile map
    TilePalatte* m_pPalatte = nullptr;
    StringId m_fileName;

public:
    TileMap();
    ~TileMap();

    bool Load(const StringId& filename) override;

    // initialization / destruction
    bool Init(const StringId& filename);
    void DestroyWorld();
    void ResetMap();

    // frame updates
    void Render(SDL_Renderer* pRenderer);

    // searches
    Vec2 GetTileMapWidthAndHeight() { return Vec2{ static_cast<const float>(m_worldWidth), static_cast<const float>(m_worldHeight) }; };
    Vec2 GetRandomWalkableTilePos() const;
    StringId GetTileTypeByLocation(float x, float y);
    int GetTileIndexByLocation(float x, float y);
    void SetTileTypeByLocation(float x, float y, TileType type);
    void SetAllTileBaseLayerToType(TileType type);
    StringId GetTileOverlayTypeByLocation(float x, float y);
    bool TileHaveOverlay(float x, float y);
    Plan GeneratePathToBestTarget(const Vec2& startPos, const Vec2& endPos) const;
    void ClearOverlay(const Vec2& location);
    bool CollectablesLeft();
    //TileDef* GetTileDefByType(TileType type);
    std::vector<std::pair<StringId, StringId>> GetMapState();

    // Snaps the xPos to the closest tile
    int SnapX(int xPos);
    int SnapY(int yPos);

    // Getters and setters for tile availablity
    void SetTileAvailabilityByLocation(float x, float y, bool isAvailable);
    bool GetTileAvailabilityByLocation(float x, float y);
    
    Vec2 GetLocationOfRandomTileByType(TileType type);

private:
    // index, grid, and world position transformation helpers
    int GetIndexFromGridPoint(int x, int y) const { return (y * m_worldWidth) + x; }
    int GetIndexFromWorldPoint(float x, float y) const;
    Vec2 GetWorldPointFromIndex(int index) const;

    // search helpers
    bool Relax(PathPlanNode* pSource, PathPlanNode* pDest, float weight) const;
    std::vector<int> GetNeighbors(int index) const;
    float GetWeightBetweenTiles(int source, int dest) const;
    void PushNeighborTileIfRoutable(int x, int y, std::vector<int>& outNeighbors) const;
};
