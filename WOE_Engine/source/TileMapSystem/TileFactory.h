/*
// TileFactory.h
#pragma once

#include "TileTypes.h"
#include "config/Settings.h"

class Tile;
class TileRenderer;

//---------------------------------------------------------------------------------------------------------------------
// Static factory for creating tiles and tile renderers.
//---------------------------------------------------------------------------------------------------------------------
class TileFactory
{
public:
    static Tile* CreateTile(int gridX, int gridY, TileType baseLayer, TileType overlay = TileType::kNone);
    static TileRenderer* CreateTileRenderer(TileType type);

private:
    static float GetWeightFromType(TileType type);
};
*/