#pragma once
#include <vector>
#include "../tools/Utilities/StringId.h"
#include "../source/TileMapSystem/TileDef.h"
#include "TileTypes.h"
#include "../tools/Utilities/tinyxml2.h"
class TileDef;


//------------------------------------------------------------------------------------------------------------------
// Tile palatte. The tilemap will use this as a palatte for creating tiles.
//------------------------------------------------------------------------------------------------------------------
class TilePalatte
{
private:
	// The palatte
	std::vector<TileDef*> m_pTileDefs;

	int m_tileWidth;
	int m_tileHeight;

public:
	TilePalatte();
	~TilePalatte();

	// Reads data from a tsx file to instantiate tile defs
	void Load(const StringId& pPalatteResource);

	// Returns a tile def at this palatte index
	TileDef* GetTileDef(int index);
	TileDef* GetTileDefByType(TileType type);

	int GetTileWidth() { return m_tileWidth; }
	int GetTileHeight() { return m_tileHeight; }
};