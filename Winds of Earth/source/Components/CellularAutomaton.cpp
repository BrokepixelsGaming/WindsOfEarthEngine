#include "CellularAutomaton.h"
#include "EngineInterface/EngineSingleton.h"
#include <iostream>

//// automaton keys
//constexpr KeyCode kRKey = 114; // Reset to empty for automaton
//constexpr KeyCode kFKey = 102; // Seed for automaton
//constexpr KeyCode kVKey = 118; // One Step of the rules
//constexpr KeyCode kTKey = 116; // Run in Loop
//constexpr KeyCode kGKey = 103; // increase size of neightborhood
//constexpr KeyCode kBKey = 98; // Decrease size of neightborhood
//// Map Keys
//constexpr KeyCode kYKey = 121; // reset
//constexpr KeyCode kHKey = 104; // seed for map generation
//constexpr KeyCode kNKey = 110; // run rules for map
//constexpr KeyCode kUKey = 117; // clean up edges
//constexpr KeyCode kJKey = 106; // run a preset pattern


///////////////////////////////////////////////////////////////
// Map Generation tuning
///////////////////////////////////////////////////////////////
constexpr float kPercentWaterToSeed = 0.005f;
constexpr float kPercentMountainToSeed = 0.0025f;
int sizeOfNeighborhoodForMapGeneration = 1;

int sizeOfNeighborhoodForAutomaton = 1;
constexpr float kSeedingPercentChance = 0.01f;
constexpr double timerDefault = 0.25;

CellularAutomaton::CellularAutomaton(Entity* owner)
	: m_pOwner(owner)
{
}

void CellularAutomaton::Update([[maybe_unused]]double deltaTimeMs)
{
	if (m_freeRun)
	{
		if (m_timer < 0)
		{
			RunRuleSet();
			m_timer = timerDefault;
		}
		else
		{
			m_timer -= deltaTimeMs;
		}
		// slowly process and step through. Maybe slow the steps down via time. 

	}
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kRKey))
	{
		// Reset the map to initial state. 
		EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->ResetMap();
	}

	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kFKey))
		SaltWorld();

	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kVKey))
	{
		// Walk through one loop of the step. 
		// Run a rule set on a set of tiles. 
		RunRuleSet();
	}

	// check if we need to flip the freerun bool. 
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kTKey))
		m_freeRun = !m_freeRun;

	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kGKey))
	{
		sizeOfNeighborhoodForAutomaton++;
	}

	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kBKey))
	{
		if (sizeOfNeighborhoodForAutomaton > 1)
		{
			sizeOfNeighborhoodForAutomaton--;
		}
	}

	// Map Generation portion***********************
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kYKey))
		ResetMapForGeneration();

	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kHKey))
		SeedMapForGeneration();

	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kJKey))
		GenerateMapDefault();
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kNKey))
		RunMapRules();


}

void CellularAutomaton::RunRuleSet()
{
	auto world = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
	auto heightAndWidth = world->GetTileMapWidthAndHeight();
	// Get a snap shot of the Map
	auto fakeMap = world->GetWorldMapCopy();
	std::cout << "Running rule set.\n";
	for (int y = 0; y < (heightAndWidth.y); ++y)
	{
		for (int x = 0; x < (heightAndWidth.x); ++x)
		{
			int index = world->GetTileIndexByLocation(static_cast<float>(x * kTileWidth), static_cast<float>(y * kTileHeight));
			if (fakeMap[index].first == "dead")
			{
				//auto aliveCount = CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kAlive, kSizeOfNeighborhood);
				if(CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kAlive, sizeOfNeighborhoodForAutomaton) == 3)
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kAlive);
			}
			if (fakeMap[index].first == "alive")
			{
				if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kAlive, sizeOfNeighborhoodForAutomaton) > 3)
				{
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kDead);
				}
				else if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kAlive, sizeOfNeighborhoodForAutomaton) < 2)
				{
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kDead);
				}
			}
		}
	}
}

void CellularAutomaton::SaltWorld()
{
	auto world = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
	auto widthAndHeight = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();

	for (int y = 0; y < (widthAndHeight.y); ++y)
	{
		for (int x = 0; x < (widthAndHeight.x); ++x)
		{
			if (m_rng.RandomPercentChance(kSeedingPercentChance))
			{
				world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kAlive);
			}
		}
	}
}

int CellularAutomaton::CountTypeInNeightborhood( FakeMap& map, Vector2<int> currentCell, TileType type, int sizeOfNeighborhood)
{
	auto world = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
	auto widthAndHeight = world->GetTileMapWidthAndHeight();
	bool isOverlay = false;

	auto startX = (currentCell.x * kTileWidth) - (sizeOfNeighborhood * kTileWidth);
	auto endX = (currentCell.x * kTileWidth) + (sizeOfNeighborhood * kTileWidth);
	auto startY = (currentCell.y * kTileHeight) - (sizeOfNeighborhood * kTileHeight);
	auto endY = (currentCell.y * kTileHeight) + (sizeOfNeighborhood * kTileHeight);
	std::string typeToCompare;
	switch(type)
	{
	case TileType::kAlive:
	{
		typeToCompare = "alive";
		break;
	}
	case TileType::kDead:
	{
		typeToCompare = "dead";
		break;
	}
	case TileType::kDying:
	{
		typeToCompare = "dying";
		break;
	}
	case TileType::kWater:
	{
		typeToCompare = "water";
		break;
	}
	case TileType::kMountain:
	{
		typeToCompare = "mountain";
		isOverlay = true;
		break;
	}
	case TileType::kForest:
	{
		typeToCompare = "forest";
		isOverlay = true;
		break;
	}
	default:
	{
		std::cout << "Error Type in CountTypeInNeightborhood()\n";
		break;
	}
	}

	int typeCount = 0;

	for (int y = startY; y <= endY; y += kTileHeight)
	{
		for (int x = startX; x <= endX; x += kTileWidth)
		{
			if (x == currentCell.x * kTileWidth && y == currentCell.y * kTileHeight)
				continue;

			// We are out of bounds. 
			if (x > (widthAndHeight.x * kTileWidth) || x < 0 || y >(widthAndHeight.y * kTileHeight) || y < 0)
				continue;

			int index = world->GetTileIndexByLocation(x, y);
			if (index < widthAndHeight.x * widthAndHeight.y && index >= 0) 
			{
				if (!isOverlay)
				{
					if (map[index].first == typeToCompare)
					{
						typeCount++;
					}
				}
				else
				{
					if (map[index].second == typeToCompare)
					{
						typeCount++;
					}
				}
			}
		}
	}

	return typeCount;
}

void CellularAutomaton::ResetMapForGeneration()
{
	// we are calling this to ensure that all overlays are cleared so we only have a base empty tile. 
	EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->ResetMap();

	auto world = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
	auto widthAndHeight = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();

	for (int y = 0; y < (widthAndHeight.y); ++y)
	{
		for (int x = 0; x < (widthAndHeight.x); ++x)
		{
			world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kAlive); // Yes it's called alive but in this I am reusing it as a "grassland"
		}
	}
}

void CellularAutomaton::SeedMapForGeneration()
{
	auto world = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
	auto widthAndHeight = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();
	for (int i = 1; i <= 4; ++i)
	{
		for (int y = 0; y < (widthAndHeight.y); ++y)
		{
			for (int x = 0; x < (widthAndHeight.x); ++x)
			{
				if (m_rng.RandomPercentChance(kPercentWaterToSeed))
				{
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kWater);
					continue;
				}
			}
		}
	}
	for (int i = 1; i <= 3; ++i)
	{
		// Get a snap shot of the Map
		auto fakeMap = world->GetWorldMapCopy();
		std::cout << "Running rule set.\n";
		for (int y = 0; y < (widthAndHeight.y); ++y)
		{
			for (int x = 0; x < (widthAndHeight.x); ++x)
			{
				int index = world->GetTileIndexByLocation(static_cast<float>(x * kTileWidth), static_cast<float>(y * kTileHeight));
				if (fakeMap[index].first == "alive")
				{
					//auto aliveCount = CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kAlive, kSizeOfNeighborhood);
					if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kWater, sizeOfNeighborhoodForMapGeneration + 1) > 3)
					{
						world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kWater);
						continue;
					}
				}
				if (fakeMap[index].first == "water")
				{
					if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kWater, sizeOfNeighborhoodForMapGeneration + 2) < 2)
					{
						world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kAlive);
						continue;
					}
				}
			}
		}
	}

}

void CellularAutomaton::GenerateMapDefault()
{
	// Reset the map
	
	ResetMapForGeneration();
	SeedMapForGeneration();

	// run the rules a set amount of times and then use clean up. 

}

void CellularAutomaton::RunMapRules()
{

	auto world = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
	auto heightAndWidth = world->GetTileMapWidthAndHeight();
	// Get a snap shot of the Map
	auto fakeMap = world->GetWorldMapCopy();
	std::cout << "Running rule set.\n";
	for (int y = 0; y < (heightAndWidth.y); ++y)
	{
		for (int x = 0; x < (heightAndWidth.x); ++x)
		{
			int index = world->GetTileIndexByLocation(static_cast<float>(x * kTileWidth), static_cast<float>(y * kTileHeight));
			if (fakeMap[index].first == "alive")
			{


				if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kMountain, sizeOfNeighborhoodForMapGeneration + 1) > 3)
				{
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kMountain);
					continue;
				}
				if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kWater, sizeOfNeighborhoodForMapGeneration + 2) > 4)
				{
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kForest);
					continue;
				}
				if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kForest, sizeOfNeighborhoodForMapGeneration + 1 ) > 3)
				{
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kForest);
					continue;
				}
			}
			if (fakeMap[index].second == "mountain")
			{
				if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kMountain, sizeOfNeighborhoodForMapGeneration) < 2)
				{
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kAlive);
					continue;
				}
			}
			if (fakeMap[index].first == "water")
			{
				if (CountTypeInNeightborhood(fakeMap, Vector2{ x,y }, TileType::kWater, sizeOfNeighborhoodForMapGeneration + 2) < 2)
				{
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kAlive);
					continue;
				}
			}
		}
	}
}
