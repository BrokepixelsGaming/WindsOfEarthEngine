// Tile.h
#pragma once

#include "..\3rdParty\include\SDL\SDL.h"
#include "TileTypes.h"
#include "PathPlan.h"
#include "config/Settings.h"
#include <Vector>
#include "../tools/Utilities/StringId.h"
#include "tools/Utilities/Vector.h"

class TileRenderer;

#include "TileDef.h"

//------------------------------------------------------------------------------------------------------------------
// The tile class, holds a non-owning reference to TileDefs for rendering and properties.
// 
// Notes:
//  - Deleting a tile means nulling the overlay.
//  - We can expose it to the game as DeleteTile() but under the hood, all its doing is nulling the overlay.
//------------------------------------------------------------------------------------------------------------------
class Tile
{
private:
    // Tile renderers; one for the base layer (the entire tile) and the other for the overlay (a smaller box over 
    // the tile.  The base layer represents the terrain while the overlay is used to show pathing information.
    // Note that the base layer cannot be nullptr, but the overlay can.
    TileDef* m_pBaseLayer;
    TileDef* m_pOverlay;

    int m_x;
    int m_y;

    // TODO(Josh): need some way of loading textures
    // Tile size/position information used for rendering.
    SDL_Rect m_baseRect;
    
    // Whether or not this tile is available for building or unit placement
    bool m_isAvailable;

public:
    Tile(int x, int y, TileDef* pBase);
    ~Tile();

    float CalculateCost() const { return (m_pBaseLayer->GetWeight() * kTileWidth); }

    void Render(SDL_Renderer* pRenderer);
    void SetBaseLayer(TileDef* pBaseLayer);
    void SetOverlay(TileDef* pOverlay);

    Vec2 GetCenter() const;
    Vec2 GetTopLeft() const;
    
    StringId GetTileType() { return m_pBaseLayer->GetType(); }
    StringId GetTileOverlayType();
    bool IsWalkable() const { return (m_pBaseLayer->GetWeight() > 0 && m_isAvailable == true); }
    bool HasOverlay() const { return (m_pOverlay != nullptr); }
    void SetIsAvailable(bool isAvailable) { m_isAvailable = isAvailable; }
    bool GetIsAvailable() { return m_isAvailable; }
};
