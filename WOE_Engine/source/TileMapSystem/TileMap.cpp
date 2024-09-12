// World.cpp
#include "TileMap.h"
#include "Tile.h"
#include "TileTypes.h"
#include "TileFactory.h"
#include "PathPlanNode.h"
#include "..\tools\Utilities\Macros.h"
#include "..\tools\Utilities\Random.h"
#include <assert.h>
#include <set>
#include <list>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "..\tools\Utilities\Vector.h"
#include "../tools/Utilities/tinyxml2.h"
#include "TilePalatte.h"
#include "TileDef.h"
#include "../tools/BleachNew.h"
#include "../tools/Utilities/tinyxml2.h"

// Josh: Commented this out
// #if _DEBUG
#include <iostream>
// #endif //_DEBUG

constexpr int kInvalidIndex = INT_MAX;
//---------------------------------------------------------------------------------------------------------------------
// Constructor.
//---------------------------------------------------------------------------------------------------------------------
TileMap::TileMap()
{
    m_pPalatte = BLEACH_NEW(TilePalatte());
}

//---------------------------------------------------------------------------------------------------------------------
// Destructor.
//---------------------------------------------------------------------------------------------------------------------
TileMap::~TileMap()
{
    DestroyWorld();
    BLEACH_DELETE(m_pPalatte);
}

bool TileMap::Load(const StringId& filename)
{
    return Init(filename);
}

//---------------------------------------------------------------------------------------------------------------------
// Initializes the world by loading the map file.
//      -filename:  The map filename.
//      -return:    true if successful, false if not.
//---------------------------------------------------------------------------------------------------------------------
bool TileMap::Init(const StringId& id)
{

 
//------------------------------------------------------------------------------------------------------------------
// Part 1 Getting the map properties
//------------------------------------------------------------------------------------------------------------------
    // Get the Tilemamp file name
    const char* fileName = id.Get().c_str();
    tinyxml2::XMLDocument document;
    document.LoadFile(fileName); // TODO: error check this
 

    // ------------Parse the data-----------------

    // Get the map element
    const tinyxml2::XMLElement* pMap = document.FirstChildElement("map");
    if (!pMap)
        std::cout << "Couldn't find map element!\n";

    // Get the attributes from the map element we want to store

    // Map width
    const tinyxml2::XMLAttribute* pMapWidth = pMap->FindAttribute("width");
    if (!pMapWidth)
        std::cout << "Couldn't find map width atrribute!\n";
    if (pMapWidth->QueryIntValue(&m_worldWidth) != tinyxml2::XML_SUCCESS)
        std::cout << "Couldn't query int value from width!\n";
    if (m_worldWidth <= 0)
        std::cout << "Map width is invalid (<=0)!\n";

    // Map height
    const tinyxml2::XMLAttribute* pMapHeight = pMap->FindAttribute("height");
    if (!pMapHeight)
        std::cout << "Couldn't find map height attribute!\n";
    if (pMapHeight->QueryIntValue(&m_worldHeight) != tinyxml2::XML_SUCCESS)
        std::cout << "Couldn't query int value from height!\n";
    if (m_worldHeight <= 0)
        std::cout << "Map height is invalid (<=0)!\n";

    // Set the map size
    m_numTiles = m_worldWidth * m_worldHeight;

    // Get the tileset
    const tinyxml2::XMLElement* pTileSet = pMap->FirstChildElement("tileset");
    if (!pTileSet)
        std::cout << "Couldn't find tileset element!\n";

    // Get the source file attribute
    const tinyxml2::XMLAttribute* pTileSetSource = pTileSet->FindAttribute("source");
    if (!pTileSetSource)
        std::cout << "Couldn't find source attribute!\n";
    const char* tilesetSource = pTileSetSource->Value();


    m_pPalatte->Load(tilesetSource); // Load the tileset

    // Get to the layer 1
    const tinyxml2::XMLElement* pLayer = pMap->FirstChildElement("layer");
    if (!pLayer)
        std::cout << "Couldn't find layer element!\n";

    // Get to the data and store it as a string
    const tinyxml2::XMLElement* pData = pLayer->FirstChildElement("data");
    std::string palatteIndexString = pData->GetText();

    // The array of palatte indexes for layer 1
    std::vector<int> palatteIndexes;

    // Loop through the string to add elements to the palatteIndex array. This loop reads the csv data as palatte indexes.
    int index = 0;
    for (size_t stringIndex = 0; stringIndex < palatteIndexString.size(); ++stringIndex)
    {
        // Skip the commas
        if (palatteIndexString[stringIndex] == ',')
            continue;

        // Skip the endlines
        if (palatteIndexString[stringIndex] == '\n')
            continue;

        // Generate the palatte index and add it to the array
        palatteIndexes.push_back((palatteIndexString[stringIndex] - 48)); // subtract by 48 to convert to int
        ++index;
    }

    // get the second layer element and repeat for the next sibling element for layer.
    // TODO(Josh): this is copy and paste so we could make this into some kind of function or use a lambda
    // Get to the layer 2
    const tinyxml2::XMLElement* pLayer2 = pLayer->NextSiblingElement("layer");
    if (!pLayer2)
        std::cout << "Couldn't find layer 2 element!\n";

    // Get to the data and store it as a string
    const tinyxml2::XMLElement* pData2 = pLayer2->FirstChildElement("data");
    std::string palatteIndexString2 = pData2->GetText();

    // The array of palatte indexes for layer 2
    std::vector<int> palatteIndexes2;

    
    // Loop through the string to add elements to the palatteIndex array. This loop reads the csv data as palatte indexes.
    index = 0;
    for (size_t stringIndex = 0; stringIndex < palatteIndexString2.size(); ++stringIndex)
    {
        // Skip the commas
        if (palatteIndexString2[stringIndex] == ',')
            continue;

        // Skip the endlines
        if (palatteIndexString2[stringIndex] == '\n')
            continue;

        // Generate the palatte index and add it to the array
        palatteIndexes2.push_back((palatteIndexString2[stringIndex] - 48)); // subtract by 48 to convert to int
        ++index;
    }
    

//------------------------------------------------------------------------------------------------------------------
// Part 2 Using the map properties to instantiate the tiles
//------------------------------------------------------------------------------------------------------------------

    
    // Loop through x and y to initialize tiles
    index = 0;
    for (int y = 0; y < m_worldHeight; ++y)
    {
        for (int x = 0; x < m_worldWidth; ++x)
        {
            assert(index < palatteIndexes.size() && index < palatteIndexes2.size());

            // using .at() to bounds check the indexes. 
            // Get the palatte index from the map's array of palatte indicies for layer 1 (base layer)
            int palatteIndex = palatteIndexes[index] - 1;
            // Layer 2
            int palatteIndex2 = palatteIndexes2[index] - 1;

            // If the tile is empty, skip
            
            if (palatteIndex == -1)
            {
                ++index;
                continue;
            }

            // Initialize the tile using the palatte.
            Tile* pTile = m_pPalatte->GetTileDef(palatteIndex)->CreateTile(x, y);
            if(palatteIndex2 >= 0)
                pTile->SetOverlay(m_pPalatte->GetTileDef(palatteIndex2));
            m_tiles.push_back(pTile);

            // Increment the index
            ++index;
        }
    }
    
    m_fileName = id;
    // Return true when finished
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
// Destroys the world.  Call Init() to reinitialize it.
//---------------------------------------------------------------------------------------------------------------------
void TileMap::DestroyWorld()
{
    for (int i = 0; i < m_numTiles; ++i)
    {
        SAFE_DELETE(m_tiles[i]);
    }
    BLEACH_DELETE(m_pPalatte);
    m_pPalatte = nullptr;
}

// Resets the tile map to an empty state. 
void TileMap::ResetMap()
{
    for (auto* pTile : m_tiles)
    {
        pTile->SetBaseLayer(m_pPalatte->GetTileDefByType(TileType::kDead));
        pTile->SetOverlay(nullptr);
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Called every frame to render the world.
//      -pRenderer: The SDL renderer to use.
//---------------------------------------------------------------------------------------------------------------------
void TileMap::Render(SDL_Renderer* pRenderer)
{
    // render the world
    for (int i = 0; i < m_numTiles; ++i)
    {
        if (m_tiles[i])
        {
            // used to debug
            //std::cout <<  "Tile Next to render: " << i << "\n";
            m_tiles[i]->Render(pRenderer);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Returns a random tile position in world space.
//      -return:    The tile position.
//---------------------------------------------------------------------------------------------------------------------
Vec2 TileMap::GetRandomWalkableTilePos() const
{
    while (true)
    {
        int index = Random::RandomRange(0, m_numTiles);

        if (m_tiles[index]->IsWalkable())
           return m_tiles[index]->GetCenter();
    }
}


StringId TileMap::GetTileTypeByLocation(float x, float y)
{
    int index = GetIndexFromWorldPoint(x, y);
    if (index == kInvalidIndex)
        return "kInvalidIndex";
    return m_tiles[index]->GetTileType();
}

int TileMap::GetTileIndexByLocation(float x, float y)
{
    return GetIndexFromWorldPoint(x, y);
}

void TileMap::SetTileTypeByLocation(float x, float y, TileType type)
{
    int index = GetIndexFromWorldPoint(x, y);
    if(type == TileType::kEmptyPath || type == TileType::kCave || type == TileType::kJungle ||type == TileType::kGrassland || type == TileType::kMountain || type == TileType::kTundra || type == TileType::kWater || type == TileType::kDesert || type == TileType::kAlive || type == TileType::kDead || type == TileType::kDying)
    {
        if (index < m_numTiles)
        {
            m_tiles[index]->SetBaseLayer(m_pPalatte->GetTileDefByType(type));
            m_tiles[index]->SetOverlay(nullptr);
        }
    }
    if (type == TileType::kTree)
    {
        if (index < m_numTiles)
        {
            m_tiles[index]->SetOverlay(m_pPalatte->GetTileDefByType(TileType::kTree));
        }
    }
    if (type == TileType::kSnowTree)
    {
        if (index < m_numTiles)
        {
            m_tiles[index]->SetOverlay(m_pPalatte->GetTileDefByType(TileType::kSnowTree));
        }
    }
    if (type == TileType::kDesertStones)
    {
        if (index < m_numTiles)
        {
            m_tiles[index]->SetOverlay(m_pPalatte->GetTileDefByType(TileType::kDesertStones));
        }
    }
    if (type == TileType::kJungleTree)
    {
        if (index < m_numTiles)
        {
            m_tiles[index]->SetOverlay(m_pPalatte->GetTileDefByType(TileType::kJungleTree));
        }
    }
    // EXAMPLE TO USE
    //if (type == TileType::kForest)
    //    if (index < m_numTiles)
    //    {
    //        m_tiles[index]->SetBaseLayer(m_pPalatte->GetTileDefByType(TileType::kForest));
    //        //m_tiles[index]->SetOverlay(m_pPalatte->GetTileDefByType(TileType::kForest));
    //    }
}

void TileMap::SetAllTileBaseLayerToType(TileType type)
{
    for (auto& tile : m_tiles)
    {
       tile->SetBaseLayer(m_pPalatte->GetTileDefByType(type));
    }
}

StringId TileMap::GetTileOverlayTypeByLocation(float x, float y)
{
    int index = GetIndexFromWorldPoint(x, y);
    if (index == kInvalidIndex)
        return "kInvalidIndex";
    return m_tiles[index]->GetTileOverlayType();
}

bool TileMap::TileHaveOverlay(float x, float y)
{
    int index = GetIndexFromWorldPoint(x, y);
    if (index == kInvalidIndex)
        return false;
    return m_tiles[index]->HasOverlay();
}

Plan TileMap::GeneratePathToBestTarget(const Vec2& startPos, const Vec2& endPos) const
{
    std::unordered_map<int, PathPlanNode*> planningNodes;
    std::vector<PathPlanNode*> goals;

    // create the start node and add it to the master list
    PathPlanNode* pStartNode = new PathPlanNode(GetIndexFromWorldPoint(startPos.x, startPos.y));
    planningNodes.emplace(pStartNode->GetTile(), pStartNode);

    // Declare the open set, which is all nodes we have yet to expand.
    typedef std::list<PathPlanNode*> OpenSet;
    OpenSet openSet;

    // add the start node
    pStartNode->SetCost(0);
    openSet.push_back(pStartNode);

    // keep going as long as there's anything in the open set
    while (!openSet.empty())
    {
        // grab the front of the open set
        PathPlanNode* pNode = openSet.front();
        openSet.pop_front();

        // Check to see if this is a node we care about.  If it is, save it for later.
        if (pNode->GetTile() == GetIndexFromWorldPoint(endPos.x, endPos.y))
            goals.push_back(pNode);
        
        // Add this vertex to the closed set.
        pNode->SetClosed();

        // get all the neighbors of this node
        std::vector<int> neighbors(GetNeighbors(pNode->GetTile()));

        // for each neighbor
        for (int neighborIndex : neighbors)
        {
            // Check to see if we've seen this node already.  If we haven't, create a node for it.
            PathPlanNode* pNeighbor = nullptr;
            auto findIt = planningNodes.find(neighborIndex);
            if (findIt == planningNodes.end())
            {
                pNeighbor = new PathPlanNode(neighborIndex);
                planningNodes.emplace(pNeighbor->GetTile(), pNeighbor);
            }
            else
            {
                pNeighbor = findIt->second;
            }

            // If this node is in the closed set, skip it.  This keeps us from processing cycles.
            if (pNeighbor->IsClosed())
                continue;

            // Relax the node.  If this path is better, we insert it into the open set.  Note that this is guaranteed 
            // to be the case the first time the node is seen because the distance is set to infinity, so this path is 
            // guaranteed to be better.
            if (Relax(pNode, pNeighbor, GetWeightBetweenTiles(pNode->GetTile(), pNeighbor->GetTile())))
            {
                openSet.push_back(pNeighbor);
            }
        }

        // sort the open set so that the front of the list is our highest priority node
        openSet.sort([](const PathPlanNode* pLeft, const PathPlanNode* pRight)
            {
                return pLeft->GetCost() < pRight->GetCost();
            });
    }

    // once we get here, we should have a list of goals
    if (goals.empty())
    {
        // destroy all the path plan nodes
        for (auto& pathPlanNodePairs : planningNodes)
        {
            delete pathPlanNodePairs.second;
        }
        return {};
    }

    // Sort the goals from furthest to closest, then remove the last goal.  This ensures that we don't select 
    // the closest goal.
    std::sort(goals.begin(), goals.end(), [](const PathPlanNode* pLeft, const PathPlanNode* pRight)
        {
            return pLeft->GetCost() > pRight->GetCost();
        });

    // build the path to the chosen goal
    Plan path;
    PathPlanNode* pNode = goals.front();
    while (pNode)
    {
        path.emplace_front(GetWorldPointFromIndex(pNode->GetTile()));
        pNode = pNode->GetPrev();
    }

    // destroy all the path plan nodes
    for (auto& pathPlanNodePairs : planningNodes)
    {
        delete pathPlanNodePairs.second;
    }

    return path;
}

void TileMap::ClearOverlay(const Vec2& location)
{
    int index = GetIndexFromWorldPoint(location.x, location.y);
    m_tiles[index]->SetOverlay(nullptr); // "Delete" the overlay by simply nulling it.
}

bool TileMap::CollectablesLeft()
{
    for (auto tile : m_tiles)
    {
        if (tile->HasOverlay())
            return true;
    }
    return false;
}

std::vector<std::pair<StringId, StringId>> TileMap::GetMapState()
{
    std::vector<std::pair<StringId, StringId>> m_tilesWithOverlays;
    for (auto tile : m_tiles)
    {
        auto base = tile->GetTileType();
        if (tile->HasOverlay())
            m_tilesWithOverlays.push_back({ base, tile->GetTileOverlayType() });
        else
            m_tilesWithOverlays.push_back({ base, "NoOverlay" });
    }
    return m_tilesWithOverlays;
}

//TileDef* TileMap::GetTileDefByType(TileType type)
//{
//    
//
//    return nullptr;
//}

//---------------------------------------------------------------------------------------------------------------------
// Returns an x position snapped to the closest tile point
//---------------------------------------------------------------------------------------------------------------------
int TileMap::SnapX(int xPos)
{
    // Get the difference 
    int difference = (xPos % m_pPalatte->GetTileWidth())  - (m_pPalatte->GetTileWidth() / 2);

    // Snap it to the grid point
    return xPos - difference;
}

//---------------------------------------------------------------------------------------------------------------------
// Returns a y position snapped to the closest tile point
//---------------------------------------------------------------------------------------------------------------------
int TileMap::SnapY(int yPos)
{
    // Get the difference 
    int difference = (yPos % m_pPalatte->GetTileHeight()) - (m_pPalatte->GetTileHeight() / 2);

    // Snap it to the grid point
    return yPos - difference;
}

//---------------------------------------------------------------------------------------------------------------------
// Set whether the tile at this world point is occupied
//---------------------------------------------------------------------------------------------------------------------
void TileMap::SetTileAvailabilityByLocation(float x, float y, bool isOccupied)
{
    // Find the index of the tile
    int index = GetIndexFromWorldPoint(x, y);

    // Set its availability
    m_tiles[index]->SetIsAvailable(isOccupied);
}

//---------------------------------------------------------------------------------------------------------------------
// Get whether the tile at this world point is occupied
//---------------------------------------------------------------------------------------------------------------------
bool TileMap::GetTileAvailabilityByLocation(float x, float y)
{
    // Find the index of the tile
    int index = GetIndexFromWorldPoint(x, y);
    return m_tiles[index]->GetIsAvailable();
}

static constexpr int kLimiterForRandom = 300;
Vec2 TileMap::GetLocationOfRandomTileByType(TileType type)
{
    int i = 0;
    while (true)
    {
        if (i > kLimiterForRandom)
            break;
        int index = Random::RandomRange(0, m_numTiles);

        if (m_tiles[index]->GetTileType() == TileTypeToString(type))
            return m_tiles[index]->GetTopLeft();
        ++i;
    }
    return Vec2(FLT_MAX, FLT_MAX);
}

//---------------------------------------------------------------------------------------------------------------------
// Gets the tile index of the tile from a world position.
//      -x:         The world X.
//      -y:         The world Y.
//      -return:    The index of the tile at this position.
//---------------------------------------------------------------------------------------------------------------------
int TileMap::GetIndexFromWorldPoint(float x, float y) const
{
    int gridY = RoundToNearest(y / m_pPalatte->GetTileHeight());
    int gridX = RoundToNearest(x / m_pPalatte->GetTileWidth());

    int index = GetIndexFromGridPoint(gridX, gridY);
    if(index > m_numTiles )
        return kInvalidIndex;
    if(index < 0)
        return kInvalidIndex;
    return index;
}

//---------------------------------------------------------------------------------------------------------------------
// Gets the world position of the center of a tile.
//      -index:     The index of the tile.
//      -return:    The world position of the tile.
//---------------------------------------------------------------------------------------------------------------------
Vec2 TileMap::GetWorldPointFromIndex(int index) const
{
    return m_tiles[index]->GetCenter();
}

bool TileMap::Relax(PathPlanNode* pSource, PathPlanNode* pDest, float weight) const
{
    if (pDest->GetCost() > pSource->GetCost() + weight)
    {
        pDest->SetCost(pSource->GetCost() + weight);
        pDest->SetPrev(pSource);
        return true;
    }

    return false;
}

std::vector<int> TileMap::GetNeighbors(int index) const
{
     std::vector<int> outNeighbors;

    int y = index / m_worldWidth;
    int x = index % m_worldWidth;

    // left neighbor
    --x;
    if (x >= 0)
        PushNeighborTileIfRoutable(x, y, outNeighbors);

    // right neighbor
    x += 2;
    if (x < m_worldWidth)
        PushNeighborTileIfRoutable(x, y, outNeighbors);

    --x;

    // upper neighbor
    --y;
    if (y >= 0)
        PushNeighborTileIfRoutable(x, y, outNeighbors);

    y += 2;
    if (y < m_worldHeight)
        PushNeighborTileIfRoutable(x, y, outNeighbors);

    return outNeighbors;
}

float TileMap::GetWeightBetweenTiles(int source, int dest) const
{
    float sourceCost = m_tiles[source]->CalculateCost();
    float destCost = m_tiles[dest]->CalculateCost();
    float totalCost = (sourceCost + destCost) / 2.f;
    return totalCost;
}

void TileMap::PushNeighborTileIfRoutable(int x, int y, std::vector<int>& outNeighbors) const
{
    int neighborIndex = GetIndexFromGridPoint(x, y);
    if (m_tiles[neighborIndex]->CalculateCost() > 0)
        outNeighbors.push_back(neighborIndex);
}
