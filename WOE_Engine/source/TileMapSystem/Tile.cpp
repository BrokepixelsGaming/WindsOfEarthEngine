// Tile.cpp
#include "Tile.h"
#include "TileRenderer.h"
#include "TileFactory.h"
#include "..\tools\Utilities\Macros.h"
#include "..\tools\Utilities\Vector.h"
#include <assert.h>
#include "..\tools\Utilities\Random.h"


//---------------------------------------------------------------------------------------------------------------------
// Constructor.
//---------------------------------------------------------------------------------------------------------------------
Tile::Tile(int x, int y, TileDef* pBase)
    : m_pBaseLayer(pBase)
    , m_pOverlay(nullptr)
    , m_x(x)
    , m_y(y)
    , m_isAvailable(true)
{
    // base rect
    m_baseRect.x = x;
    m_baseRect.y = y;
    m_baseRect.w = pBase->GetWidth() - 1;
    m_baseRect.h = pBase->GetHeight() - 1;
}

//---------------------------------------------------------------------------------------------------------------------
// Destructor.
//---------------------------------------------------------------------------------------------------------------------
Tile::~Tile()
{
}

//---------------------------------------------------------------------------------------------------------------------
// Renders a single tile by telling both of its layers to render.
//     -pRenderer: The SDL renderer to use.
//---------------------------------------------------------------------------------------------------------------------
void Tile::Render([[maybe_unused]]SDL_Renderer* pRenderer)
{
    // no base layer
    if (!m_pBaseLayer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No base layer on tile.");
        return;
    }

    // render the base layer
    m_pBaseLayer->Render(m_x, m_y);

    // render the overlay
    if (m_pOverlay)
        m_pOverlay->Render(m_x, m_y);
}


//---------------------------------------------------------------------------------------------------------------------
// Sets the base layer for this tile. (DEPRECATED)
//     -type: The new base layer type.
//---------------------------------------------------------------------------------------------------------------------
void Tile::SetBaseLayer([[maybe_unused]] TileDef* pBaseLayer )
{

    m_pBaseLayer = pBaseLayer;
}

//---------------------------------------------------------------------------------------------------------------------
// Sets the overlay for this tile.
//     -type: The new overlay type.
//---------------------------------------------------------------------------------------------------------------------
void Tile::SetOverlay(TileDef* pOverlay)
{
    m_pOverlay = pOverlay;
}

//---------------------------------------------------------------------------------------------------------------------
// Gets the pixel position of the center of the tile.
//      -return:    The world position of the center of the tile.
//---------------------------------------------------------------------------------------------------------------------
Vec2 Tile::GetCenter() const
{
    return Vec2(m_baseRect.x + (m_baseRect.w / 2.f), m_baseRect.y + (m_baseRect.h / 2.f));
}

Vec2 Tile::GetTopLeft() const
{
    return Vec2(static_cast<float>(m_baseRect.x), static_cast<float>(m_baseRect.y));
}

StringId Tile::GetTileOverlayType()
{
    if(m_pOverlay != nullptr)
       return m_pOverlay->GetType();
    return nullptr;
}
