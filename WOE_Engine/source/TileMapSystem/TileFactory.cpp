// TileFactory.cpp
/*
#include "TileFactory.h"
#include "Tile.h"
#include "TileRenderer.h"
#include <assert.h>

//---------------------------------------------------------------------------------------------------------------------
// Creates a tile.  All tiles should be created using this interface.
//     -gridX: The X location of the tile on the grid.
//     -gridY: The Y location of the tile on the grid.
//     -baseLayer: The base layer for this tile.
//     -overlay: The overlay for this tile.  Defaults to TileType::k_none, meaning there will be no overlay.
//     -return: A full constructed Tile, or nullptr if it failed.
//---------------------------------------------------------------------------------------------------------------------


Tile* TileFactory::CreateTile(int gridX, int gridY, TileType baseLayer, TileType overlay)
{
    // we need to have a base layer
    if (baseLayer == TileType::kNone)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "base layer of tile can't be k_none.");
        return nullptr;
    }

    // create and set the tile
   // Tile* pTile = new Tile(gridX * kTileWidth, gridY * kTileHeight, GetWeightFromType(baseLayer));

    // set the tile & overlay
    //pTile->SetBaseLayer(baseLayer);
   // pTile->SetOverlay(overlay);

    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
// Creates a tile renderer.  All tile renderers should be created using this interface.
//     -tileType: The type of renderer to create.
//     -return: A full constructed TileRenderer, or nullptr if it failed.
//---------------------------------------------------------------------------------------------------------------------
TileRenderer* TileFactory::CreateTileRenderer(TileType type)
{
    TileRenderer* pRenderData = nullptr;

    // Note: In a real game, this would be loaded from a file or palette.  I'm doing this purely for 
    // instructional purposes.  If I created some kind of architecture or file loading scheme, it would 
    // obfuscate the code.
    switch (type)
    {
        //=====================================================================
        // Normal Tiles.

    case TileType::kGrass:
        pRenderData = new TileRenderer(0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE);
        break;

    case TileType::kForest:
        pRenderData = new TileRenderer(0x00, 0x77, 0x00, SDL_ALPHA_OPAQUE);
        break;

    case TileType::kRiver:
        pRenderData = new TileRenderer(0x00, 0x00, 0xff, SDL_ALPHA_OPAQUE);
        break;
    case TileType::kWall:
        pRenderData = new TileRenderer(0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        break;
    case TileType::kEmptyPath:
        pRenderData = new TileRenderer(0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        break;
        //=====================================================================
        // Overlay Tiles.

    case TileType::kStart:
        pRenderData = new TileRenderer(0xff, 0xff, 0x00, SDL_ALPHA_OPAQUE);
        break;

    case TileType::kPath:
        pRenderData = new TileRenderer(0xff, 0x00, 0xff, SDL_ALPHA_OPAQUE);
        break;

    case TileType::kOpenSet:
        pRenderData = new TileRenderer(0x00, 0xff, 0xff, SDL_ALPHA_OPAQUE);
        break;

    case TileType::kClosedSet:
        pRenderData = new TileRenderer(0x00, 0x77, 0x77, SDL_ALPHA_OPAQUE);
        break;

    case TileType::kSmallOrb:
        pRenderData = new TileRenderer(0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
        break;
    case TileType::kLargeOrb:
        pRenderData = new TileRenderer(0xff, 0xff, 0x00, SDL_ALPHA_OPAQUE);
        break;

        //=====================================================================
        // Error

    default:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid tile type.", SDL_GetError());
        break;
    }

    return pRenderData;
}

//---------------------------------------------------------------------------------------------------------------------
// An internal function that gets the weight of a tile based on the type.  This replaces a data file....
//     -type: The type of tile.
//     -return: The weight of that tile.
//---------------------------------------------------------------------------------------------------------------------
float TileFactory::GetWeightFromType(TileType type)
{
    switch (type)
    {
    case TileType::kGrass:  return 1.f;
    case TileType::kForest: return 10.f;
    case TileType::kRiver:  return 0;
    case TileType::kWall: return 0;
    case TileType::kEmptyPath: return 10.f;
    case TileType::kLargeOrb: return 1.f;
    case TileType::kSmallOrb: return 5.f;

    default:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid tile type for weight.");
        return 1.f;
    }
}
*/


