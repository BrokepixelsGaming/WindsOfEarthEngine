#include "WorldGenerationManager.h"
#include "EngineInterface/EngineSingleton.h"
#include "SimpleInstrumentationProfiler.h"

#define _MAP_LOGGING_ 0

#if _MAP_LOGGING_
#include <iostream>
#endif

// Noise Freqs
static constexpr float kHeatNoiseFrequency = 0.5f;
static constexpr float kHeatModifiedFrequency = 10.0f;
static constexpr float kHeightMapFrequency = 5.5f;
static constexpr float kTileMapFrequency = 3.0f;

// Selector Size
constexpr int kSelectionSize = 10;
constexpr int kSelectionTotalSize = kSelectionSize * kSelectionSize;

// Tree % 
static constexpr float kTundraTreePercent = 0.10f;
static constexpr float kSnowTreePercent = 0.25f;
static constexpr float kDesertStonePercent = 0.05f;
static constexpr float kJungleTreePercent = 0.45f;
static constexpr float kGrasslandTreePercent = .30f;

// Cave % limits
static constexpr float kMountainPercentForCaves = 0.15f;
static constexpr float kMountainPerCavePercent = 0.03f;
static constexpr int kRoomMinHeightSize = 3;
static constexpr int kRoomMaxHeightSize = 5;
static constexpr int kRoomMinWidthSize = 3;
static constexpr int kRoomMaxWidthSize = 5;

// using whole number due to floating point error. When using .01f it would end up 0.00999999XXX during runtime which has the potential
// to throw the math off that I am trying to use. Now this may not have a direct effect but I didn't want the error while trying to figure
// this out. 
static constexpr float kPercentPerPixel =  1.0f;

WorldGenerationManager::WorldGenerationManager(Entity* pOwner)
	: m_pOwner(pOwner)
	, m_baseMapGenerator(pOwner)
    , m_worldTexture(nullptr)
    , m_direction(CardnalDirection::kNorth)
{
   auto widthAndHeight = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();
   m_surface = SDL_CreateRGBSurface(0, static_cast<int>(widthAndHeight.x), static_cast<int>(widthAndHeight.y), 32, 0, 0, 0, 0);
   EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->ResetMap();
}

WorldGenerationManager::~WorldGenerationManager()
{
    // Clean up
    if(m_worldTexture)
        SDL_DestroyTexture(m_worldTexture);
    if(m_surface)
        SDL_FreeSurface(m_surface);

    m_worldTexture = nullptr;
    m_surface = nullptr;
}

void WorldGenerationManager::GenerateGlobalHeatMap()
{
    auto widthAndHeight = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();
    ValueMap heatMap(static_cast<const unsigned __int64>(widthAndHeight.x), std::vector<double>(static_cast<const unsigned __int64>(widthAndHeight.y), 0.0));

    for (int x = 0; x < widthAndHeight.x; ++x)
    {
        for (int y = 0; y < widthAndHeight.y; ++y)
        {
            // Scale the coordinates
            double scale = 0.1;
            double nx = static_cast<double>(x) * scale;
            double ny = static_cast<double>(y) * scale;

            // Modify the Perlin noise input to create the desired effect.
            double modifiedX = nx + kHeatNoiseFrequency * sin(ny * kHeatNoiseFrequency);
            double modifiedY = ny + kHeatNoiseFrequency * sin(nx * kHeatNoiseFrequency);
            
            heatMap[x][y] = m_rng.Perlin(static_cast<float>(modifiedX / widthAndHeight.x) * kHeatModifiedFrequency, static_cast<float>(modifiedY / widthAndHeight.y) * kHeatModifiedFrequency, m_rng.GetCurrentSeedUInt());
        }
    }
    m_heatMap = heatMap;
}

void WorldGenerationManager::GenerateGlobalHeightMap()
{
    auto widthAndHeight = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();
    ValueMap heightMap(static_cast<const unsigned __int64>(widthAndHeight.x), std::vector<double>(static_cast<const unsigned __int64>(widthAndHeight.y), 0.0));
    // Generate the modified heat map using Perlin noise
    for (int x = 0; x < widthAndHeight.x; ++x)
    {
        for (int y = 0; y < widthAndHeight.y; ++y)
        {
            heightMap[x][y] = m_rng.Perlin((x / widthAndHeight.x) * kHeightMapFrequency, (y / widthAndHeight.y) * kHeightMapFrequency, m_rng.GetCurrentSeedUInt());
        }
    }
    m_heightMap = heightMap;
}

void WorldGenerationManager::GenerateGlobalMapView()
{
    // Access the pixel data
    Uint32* pixels = static_cast<Uint32*>(m_surface->pixels);
    auto widthAndHeight = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();
    Color color = {0,0,0,0};
    for (int x = 0; x < widthAndHeight.x; ++x)
    {
        for (int y = 0; y < widthAndHeight.y; ++y)
        {
            // Figure out what color it needs to be
            double height = m_heightMap[x][y];
            double heat = m_heatMap[x][y];

            ///////////////////////////////////////////////////////////////
            // This is based on a height map so this is just a compairson to figure out which biome
            // will be placed. 
            ///////////////////////////////////////////////////////////////
            // if deep ocean 
            if (height < 0.3)
            {
                // checking to see if it will be frozen. 
                if (heat < .2)
                    color = kColorSapphire;
                else
                    color = kColorDarkBlue;
            }
            // if ocean
            else if (height < .45)
            {
                if (heat < .25)
                    color = kColorIceBlue;
                else
                    color = kColorBlue;
            }
            // if grassland
            else if (height < .78)
            {
                if (heat < .25)
                    color = kColorWhite;
                else if (heat < .45)
                    color = kColorGreen;
                else if (heat < .55)
                    color = kColorDarkGreen;
                else if (heat < .75)
                    color = kColorGreen;
                else if (heat < 1.0)
                    color = kColorLightBeige;
            }
            // if mountain
            else if (height < .8)
            {
                if (heat < .25)
                    color = kColorGrey;
                else if (heat < .75)
                    color = kColorBrown;
                else if (heat < 1.0)
                    color = kColorLightBrown;
            }

            // Color the pixel. 
            pixels[y * m_surface->w + x] = SDL_MapRGBA(m_surface->format, color.red, color.green, color.blue, color.alpha);
            m_pixelsInSurface[y * m_surface->w + x] = color; // This is to allow for easy access to color data so I can grab the entire color.
        }
    }
    m_worldTexture = SDL_CreateTextureFromSurface(EngineSingleton::Get()->GetWindow()->GetRenderer(), m_surface);
}

void WorldGenerationManager::GenerateTileMap()
{
    if (m_activeSelection)
    {
        // I shouldn't need bounds checking as I have to be within bounds in order for m_activeSelection to be true; 
        // else it won't move or be true;
        
        // Gather a list of all the pixels in the selected area.
        Color selectedColors[kSelectionSize * kSelectionSize] = {0,0,0,0};
        int index = 0;
        for (int x = static_cast<int>(m_selectorLocation.x - m_worldTextureTransform.x); x < ((m_selectorLocation.x - m_worldTextureTransform.x) + kSelectionSize); ++x)
        {
            for (int y = static_cast<int>((m_selectorLocation.y - m_worldTextureTransform.y)); y < ((m_selectorLocation.y - m_worldTextureTransform.y) + kSelectionSize); ++y)
            {
                selectedColors[index] = m_pixelsInSurface[y * m_surface->w + x];
                index++;
            }
        }

        // Now we have a sample of the colors that I need. 
        // We are going to compile the data into a usable format. In this case I am going to be talking about % out of 100
        // such that you would have [{green, 60.0f}, {DarkGreen, 40.0f}] which would meean that 40% is jungle and 60% is grasslands.
        std::map<Color, float> biomePercents;

        for (auto& color : selectedColors)
        {
            auto it = biomePercents.find(color);
            if (it != biomePercents.end())
            {
                it->second += kPercentPerPixel;
            }
            else
                biomePercents.emplace(color, kPercentPerPixel);
        }
#if _MAP_LOGGING_
        // make a percentage based on pixel color such as 60% Grassland 20%water 20% mountain. using 10 X 10 selecction makes this easy. 
        // As each pixel would contribute .01. 
        for (auto& biome : biomePercents)
        {
            PrintColorToConsole(biome.first);
            std::cout << " " << biome.second << "%\n";
        }
#endif

        // With these percentages I will create a tile map still using the current seed.
        // Percentages will change the percentage of tiles on the maps. 
        ProcessTileMap(biomePercents);
    }
}

// We have the selectionData from the overworld map and we know the location that was selected. 
void WorldGenerationManager::ProcessTileMap(std::map<Color, float>& selectionData)
{
    auto widthAndHeight = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();
    ///////////////////////////////////////////////////////////////
    // Generating the base Layers for the tile map. 
    ///////////////////////////////////////////////////////////////
    // this does two things one it sorts the % from high to low while also converting the Color to a TileType to use when placing tiles. 
    std::map<float, TileType, std::greater<float>> processedData;
    for (auto& data : selectionData)
    {
        processedData.emplace(data.second, ColorToTileType(data.first));
    }
    // this would mean that we are solidly in only one biome there would be no traces of others. 
    if (selectionData.size() == 1)
    {
        const auto& data = *selectionData.begin();
        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetAllTilesToType(ColorToTileType(data.first));
    }
    else
    {
        // here we will grab the first element which will be the majority of the map. 
        auto it = processedData.begin();
        const auto& data = *it;
        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetAllTilesToType(data.second);        
        // Generate the seed for the tileMap
        const unsigned int tileMapNoiseSeed = static_cast<unsigned int>(Square(m_selectorLocation.x) + Square(m_selectorLocation.y));

        // Now we will need to process the transition biomes which are not the majority.  
        ++it;
        if (it != processedData.end())
        {
            const auto& data2 = *it;
            // Generate some Perlin noise and set the tiles based on the selection location. 
            for (int x = 0; x < widthAndHeight.x; ++x)
            {
                for (int y = 0; y < widthAndHeight.y; ++y)
                {
                    float val = m_rng.Perlin((x / widthAndHeight.x) * kTileMapFrequency, (y / widthAndHeight.y) * kTileMapFrequency, (tileMapNoiseSeed));
                    if (val < (data2.first / kSelectionTotalSize))
                    {
                        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileTypeByLocation((x * kTileWidth), (y * kTileHeight), data2.second);
                    }
                }
            }
        }
        // If there is a 3rd type Lets do something with it. 
        // We check the next iterator and see if we have reached the end. If there is still another element we will process it.
        // NOTE: I don't plan to include 4 at this time. If there is a 4th element in the processed data I just don't care about it.
        ++it;
        if (it != processedData.end())
        {
            // Need to check that we have something before accessing. 
            auto& data3 = *it;
            for (int x = 0; x < widthAndHeight.x; ++x)
            {
                for (int y = 0; y < widthAndHeight.y; ++y)
                {
                    // Scale the coordinates
                    double scale = .1;
                    double nx = static_cast<double>(x) * scale;
                    double ny = static_cast<double>(y) * scale;

                    // Modify the Perlin noise input to create the desired effect.
                    float modifiedX = static_cast<float>(Square(nx));
                    float modifiedY = static_cast<float>(Square(ny));

                    float val = m_rng.Perlin((modifiedX / widthAndHeight.x) * kTileMapFrequency, (modifiedY / widthAndHeight.y) * kTileMapFrequency, (tileMapNoiseSeed));
                    if (val < (data3.first / kSelectionTotalSize))
                    {
                        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileTypeByLocation((x * kTileWidth), (y * kTileHeight), data3.second);
                    }
                }
            }
        }
    }
    ///////////////////////////////////////////////////////////////
    // Start placing things on the Map. 
    ///////////////////////////////////////////////////////////////
    for (int x = 0; x < widthAndHeight.x; ++x)
    {
        for (int y = 0; y < widthAndHeight.y; ++y)
        {
            auto tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(static_cast<float>(x * kTileWidth), static_cast<float>(y * kTileHeight));
            // random chance to be true that is dependant on the tileType. Each biome has it's own % for trees. 
            if (PlaceTileOverlay(tileType))
            {
                PlaceObjectBasedOnTileTypeAtLocation(tileType, x * kTileWidth, y * kTileHeight);
            }
        }
    }
    ///////////////////////////////////////////////////////////////
    //  Here I am gonna Generate caves if we have mountains
    ///////////////////////////////////////////////////////////////
    for (auto& data : processedData)
    {
        if(data.second == TileType::kMountain)
            if (data.first > (kMountainPercentForCaves * 100.0f))
            {
                float percentMountain = data.first / 100.0f;
                while (percentMountain > kMountainPerCavePercent)   
                {
                    auto location = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetLocationOfRandomTileByType(TileType::kMountain);
                    DigA3x3CaveRoom(location);
                    DigUntilEdgeOrNewBiome(location);
                    percentMountain -= kMountainPerCavePercent;
                }
            }
    }
}

void WorldGenerationManager::PrintColorToConsole(const Color color)
{
    if (color == kColorBlack)
        std::cout << "Black";
    else if (color == kColorGreen)
        std::cout << "Green";
    else if (color == kColorBlue)
        std::cout << "Blue";
    else if (color == kColorBrown)
        std::cout << "Brown";
    else if (color == kColorDarkBlue)
        std::cout << "DarkBlue";
    else if (color == kColorDarkBrown)
        std::cout << "DarkBrown";
    else if (color == kColorDarkGreen)
        std::cout << "DarkGreen";
    else if (color == kColorGrey)
        std::cout << "Grey";
    else if (color == kColorIceBlue)
        std::cout << "IceBlue";
    else if (color == kColorIceGrey)
        std::cout << "IceGrey";
    else if (color == kColorLightBeige)
        std::cout << "LightBeige";
    else if (color == kColorLightBlue)
        std::cout << "LightBlue";
    else if (color == kColorLightBrown)
        std::cout << "LightBrown";
    else if (color == kColorLightGreen)
        std::cout << "LightGreen";
    else if (color == kColorRed)
        std::cout << "Red";
    else if (color == kColorSapphire)
        std::cout << "Sapphire";
    else if (color == kColorWhite)
        std::cout << "White";
    else 
        std::cout << "Unknown Color";
}

TileType WorldGenerationManager::ColorToTileType(Color color)
{
    if (color == kColorGreen)
        return TileType::kGrassland;
    else if (color == kColorBlue)
        return TileType::kWater;
    else if (color == kColorDarkBlue)
        return TileType::kWater;
    else if (color == kColorDarkGreen)
        return TileType::kJungle;
    else if (color == kColorIceBlue)
        return TileType::kIceSheet;
    else if (color == kColorSapphire)
        return TileType::kIceSheet;
    else if (color == kColorBrown)
        return TileType::kMountain;
    else if (color == kColorLightBrown)
        return TileType::kMountain;
    else if (color == kColorGrey)
        return TileType::kMountain;
    else if (color == kColorWhite)
        return TileType::kTundra;
    else if (color == kColorLightBeige)
        return TileType::kDesert;
    else
        return TileType::kInvalid;
}

// 
bool WorldGenerationManager::PlaceTileOverlay(StringId& type)
{
    if (type == "grassland")
        return m_rng.RandomPercentChance(kGrasslandTreePercent);
    else if (type == "desert")
        return m_rng.RandomPercentChance(kDesertStonePercent);
    else if (type == "tundra")
        return m_rng.RandomPercentChance(kTundraTreePercent);
    else if (type == "jungle")
        return m_rng.RandomPercentChance(kJungleTreePercent);
    else
        return false;
}

void WorldGenerationManager::PlaceObjectBasedOnTileTypeAtLocation(StringId& type, int x, int y)
{
    if (type == "grassland")
        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileTypeByLocation(x, y, TileType::kTree);
    else if (type == "desert")
        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileTypeByLocation(x, y, TileType::kDesertStones);
    else if(type == "tundra")
        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileTypeByLocation(x, y, TileType::kSnowTree);
    else if (type == "jungle")
        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileTypeByLocation(x, y, TileType::kJungleTree);
}

// This is just a starting cave. From here it should grow randomly. 
void WorldGenerationManager::DigA3x3CaveRoom(Vec2 locationOfCenterTile)
{
    // Determine the height and width of the room 
    int roomHeight =3;
    int roomWidth = 3;

    // This is so I can figure how many steps I need to take towards top left. 
    Vec2 centerOfRoom = locationOfCenterTile;

    // We are starting in the top left. 
    Vec2 pointToDig = centerOfRoom + Vec2{ -(kTileWidth), -(kTileHeight) };

    // Here we dig the room. 
    for (int height = 0; height < roomHeight; ++height)
    {
        for (int width = 0; width < roomWidth; ++width)
        {
            DigTile(pointToDig, TileType::kCave);
            pointToDig += {kTileWidth, 0 };
        }
        // Go back to the left.
        pointToDig += {-(kTileWidth * static_cast<float>(roomWidth)), kTileHeight};
    }
}

// We will dig out the single cave room into a edge or a different biome. 
void WorldGenerationManager::DigUntilEdgeOrNewBiome(Vec2 location)
{
    // Get starting point. 
    Vec2 digPoint = location;
    // Grab the world since we will need it alot. 
    auto pWorld = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
    //If tile is not Mountain or Cave we are done. 
    while (pWorld->GetTileTypeByLocation(digPoint.x, digPoint.y) == "mountain" || pWorld->GetTileTypeByLocation(digPoint.x, digPoint.y) == "cave")
    {
        // if we are mountain we will make it a cave. 
        if (pWorld->GetTileTypeByLocation(digPoint.x, digPoint.y) == "mountain")
            DigTile(digPoint, TileType::kCave);

        // Pick a direction
        RandomlyChangeDirections();
        switch (m_direction)
        {
            case kWest:     digPoint += {-kTileWidth, 0};   break;
            case kNorth:    digPoint += {0, kTileHeight};   break;
            case kEast:     digPoint += {kTileWidth, 0};    break;
            case kSouth:    digPoint += {0, -kTileHeight};  break;
        }
    }
}

void WorldGenerationManager::DigTile(Vec2 location, TileType type)
{
    EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileTypeByLocation(location.x, location.y, type);
}

void WorldGenerationManager::RandomlyChangeDirections()
{
    // Here I prevent the direction from going in the direct opposite way. 
    bool chosen = false;
    int choice = m_rng.RandomInRange(0, 3);
    while (!chosen)
    {
        if (choice == 0 && m_direction == kEast)
        {
            choice = m_rng.RandomInRange(0, 3);
            continue;
        }
        if (choice == 1 && m_direction == kSouth)
        {
            choice = m_rng.RandomInRange(0, 3);
            continue;
        }
        if (choice == 2 && m_direction == kWest)
        {
            choice = m_rng.RandomInRange(0, 3);
            continue;
        }
        if (choice == 3 && m_direction == kNorth)
        {
            choice = m_rng.RandomInRange(0, 3);
            continue;
        }
        // sets to leave the while loop. 
        chosen = true;
    }
    // Setting the direction of travel. 
    assert(choice >= 0 && choice < kNumCardinalDirections);
    m_direction = static_cast<CardnalDirection>(choice);
}

void WorldGenerationManager::Update([[maybe_unused]] double deltaTimeMs)
{
    if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kRKey))
    {
        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->ResetMap();
        SDL_DestroyTexture(m_worldTexture);
        m_activeSelection = false;
        m_worldTexture = nullptr;
        m_rng.ChangeSeedRandomly();
    }
    if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kFKey))
    {
        if (m_worldTexture)
        {
            SDL_DestroyTexture(m_worldTexture);
            m_worldTexture = nullptr;
            m_rng.ChangeSeedRandomly();
        }
        //m_baseMapGenerator.GenerateHeightMapUsingNoise();
        SimpleInstrumentationProfiler MapProfiler("GenerateMapData");
        GenerateGlobalHeatMap();
        GenerateGlobalHeightMap();
        GenerateGlobalMapView();
    }
    if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kGKey))
        m_activeSelection = false;

    if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kBKey))
    {
        EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->ResetMap();
        GenerateTileMap();
    }
}

void WorldGenerationManager::Render(SDL_Renderer* pRenderer, [[maybe_unused]]Vec2 cameraOffset)
{
    if (m_worldTexture != nullptr)
    {
        // Get the location of the owner. 
        SDL_RenderCopyF(pRenderer, m_worldTexture, NULL, &m_worldTextureTransform);
    }
   
    if (m_activeSelection)
    {
        Vector2 topLeft = { m_selectorLocation.x, m_selectorLocation.y };
        Vector2 topRight = { m_selectorLocation.x + kSelectionSize, m_selectorLocation.y };
        Vector2 bottomLeft = {m_selectorLocation.x, m_selectorLocation.y + kSelectionSize };
        Vector2 bottomRight = {m_selectorLocation.x + kSelectionSize, m_selectorLocation.y + kSelectionSize };

        SDL_SetRenderDrawColor(pRenderer, kColorRed.red, kColorRed.green, kColorRed.blue, kColorRed.alpha);
        // top left corner to bottom left. 
        SDL_RenderDrawLine(pRenderer, static_cast<int>(topLeft.x), static_cast<int>(topLeft.y), static_cast<int>(bottomLeft.x), static_cast<int>(bottomLeft.y));
        // top Left corner to top Right
        SDL_RenderDrawLine(pRenderer, static_cast<int>(topLeft.x), static_cast<int>(topLeft.y), static_cast<int>(topRight.x ), static_cast<int>(topRight.y));
        // top right corner to bottom Right
        SDL_RenderDrawLine(pRenderer, static_cast<int>(topRight.x), static_cast<int>(topRight.y), static_cast<int>(bottomRight.x), static_cast<int>(bottomRight.y));
        // bottom right corner to bottom left
        SDL_RenderDrawLine(pRenderer, static_cast<int>(bottomRight.x), static_cast<int>(bottomRight.y ), static_cast<int>(bottomLeft.x ), static_cast<int>(bottomLeft.y));
    }
    SDL_SetRenderDrawColor(pRenderer, kColorBlack.red, kColorBlack.green, kColorBlack.blue, kColorBlack.alpha);
}

bool WorldGenerationManager::ProcessEvent(Event& evt)
{
    auto sdlEvt = evt.GetSDLEvent();
    if (sdlEvt)
    {
        switch (sdlEvt->type)
        {
        case SDL_MOUSEBUTTONUP:
        {
            //std::cout << sdlEvt->button.button;
            // '\x1' is left click '\x3' is right click
            if (sdlEvt->button.button == '\x1') 
            {

                // Check if they overlap horizontally.
                bool xBounds = m_worldTextureTransform.x < evt.GetSDLEvent()->button.x && evt.GetSDLEvent()->button.x < (m_worldTextureTransform.x + m_worldTextureTransform.w) - kSelectionSize;

                // Check if they overlap vertically.
                bool yBounds = m_worldTextureTransform.y < evt.GetSDLEvent()->button.y && evt.GetSDLEvent()->button.y < (m_worldTextureTransform.y + m_worldTextureTransform.h) - kSelectionSize;

                if (xBounds && yBounds)
                {
                    m_selectorLocation.x = static_cast<float>(evt.GetSDLEvent()->button.x);
                    m_selectorLocation.y = static_cast<float>(evt.GetSDLEvent()->button.y);
                    m_activeSelection = true;
                }
            }
        }
        }
    }
    return false;
}
