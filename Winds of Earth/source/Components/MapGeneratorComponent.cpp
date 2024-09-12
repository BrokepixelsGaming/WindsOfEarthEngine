#include "MapGeneratorComponent.h"
#include "EngineInterface/EngineSingleton.h"
#include <cmath>
#include <SimpleInstrumentationProfiler.h>

#if THREADS 
#include <thread>
constexpr size_t kNumThreads = 10;
#endif

// used for Noise normalization.
constexpr unsigned int kUnsignedIntMax = SDL_MAX_UINT32;
constexpr int kNoiseSeed = 1234162; // seed for the map. 

constexpr unsigned int kSeedMin = 1;
constexpr unsigned int kSeedMax = kUnsignedIntMax;

// this is index location based. 
constexpr int kXStart = 0;
constexpr int kYStart = 0;
constexpr float kNoiseInputRange = 15.5f;

//***************************************//
//        Tile Noise Limit Values        //
//***************************************//
constexpr float kWaterNoiseLimit = 0.5f;
constexpr float kBeachNoiseLimit = 1.5f;
constexpr float kGrasslandNoiseLimit = 0.74f;
constexpr float kDesertNoiseLimit = 1.65f;
constexpr float kTundraNoiseLimit = 1.75f;
constexpr float kMountainNoiseLimit = 1.0f;
//****************************************//
MapGeneratorComponent::MapGeneratorComponent(Entity* pOwner)
	: m_pOwner (pOwner)
{
	
}

void MapGeneratorComponent::Update([[maybe_unused]]double deltaTimeMs)
{
	//if (!m_hasRun)
	//{
	//	m_hasRun = true;

	//	GenerateHeightMapUsingNoise();
	//}
}

void MapGeneratorComponent::GenerateHeightMapUsingNoise(unsigned int seed)
{
	if (seed == 0)
	{
		seed = m_rng.RandomInRange(kSeedMin, kSeedMax);
		
	}
	m_rng.SetSeed(seed);
	auto world = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
	// Loop through the entire map while setting the different tiles. 
	auto heightAndWidth = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();

// Use this to turn on and off threading. Trying to get more performance without disrupting the good portion of the solution.
// I am also able to prove now that the Threads actually improve performance in the time it takes to complete.
// Unthreaded takes 20.XXX+ where with 10 threads I can get it to consistently down to 6.xxx this would see greater return in a bigger world. But due to the limitations
// of my engine I am keeping it large adjacently 
#if THREADS

#if _LOGGING_
	SimpleInstrumentationProfiler counter("time to thread.");
#endif 
	// total length based on size of map. 
	int length = static_cast<int>(heightAndWidth.x * heightAndWidth.y);
	// number of tiles each Thread should deal with. 
	int stride = length / kNumThreads;

	// Thread Function
	auto threadFunc = [heightAndWidth, this, &world, &seed](int start, int end)
		{
			// now we need to get the x and y for each of the indexs and then generate the noise to go along with it. ALL of this is still isolated to the thread. 
			for (int i = start; i < end; ++i)
			{
				int row = i / static_cast<int>(heightAndWidth.x); // Calculate the row number
				int col = i % static_cast<int>(heightAndWidth.x); // Calculate the column number

				int x = col; // Calculate the X coordinate
				int y = row; // Calculate the Y coordinate

				float noise = m_rng.Perlin((x / heightAndWidth.x) * kNoiseInputRange, (y / heightAndWidth.y) * kNoiseInputRange, seed);

				if (noise < kWaterNoiseLimit)
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kWater);
				else if (noise < kGrasslandNoiseLimit)
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kGrassland);
				else if (noise < kMountainNoiseLimit)
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kMountain);
				else if (noise < kBeachNoiseLimit)
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kBeach);
				else if (noise < kDesertNoiseLimit)
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kDesert);
				else if (noise < kTundraNoiseLimit)
					world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kTundra);

			}
		};

	int start = 0;
	int end = stride;

	// Time to spin up all the threads and get them working. 
	std::thread workers[kNumThreads - 1];
	for (size_t i = 0; i < kNumThreads - 1; ++i)
	{
		workers[i] = std::thread(threadFunc, start, end);
		start += stride;
		end += stride;
	}

	threadFunc(start, (static_cast<int>(heightAndWidth.x * heightAndWidth.y)));

	for (size_t i = 0; i < kNumThreads - 1; ++i)
	{
		workers[i].join();
	}

#else

#if _LOGGING_
	SimpleInstrumentationProfiler counter("counter for non-thread time.");
#endif

	float noise = 0.0f;
	// Here we are going to walk thru every tile. 
	for (int y = kYStart; y < (heightAndWidth.y ); ++y)
	{
		for (int x = kXStart; x < (heightAndWidth.x); ++x)
		{
			
			noise = m_rng.Perlin((x / heightAndWidth.x) * kNoiseInputRange, (y / heightAndWidth.y) * kNoiseInputRange, kNoiseSeed);
			if (noise < kWaterNoiseLimit)
			{
				world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kWater);
				continue;
			}
			if (noise < kBeachNoiseLimit)
			{
				world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kBeach);
				continue;
			}
			if(noise < kGrasslandNoiseLimit)
			{
				world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kGrassland);
				continue;
			}
			if (noise < kDesertNoiseLimit)
			{
				world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kDesert);
				continue;
			}
			if (noise < kTundraNoiseLimit)
			{
				world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kTundra);
				continue;
			}
			if (noise < kMountainNoiseLimit)
			{
				world->SetTileTypeByLocation(x * kTileWidth, y * kTileHeight, TileType::kMountain);
				continue;
			}
		}
	}
#endif // 0
}

float MapGeneratorComponent::NormalizeNoise(unsigned int noise)
{
	return static_cast<float>(noise) / static_cast<float>(kUnsignedIntMax);
}
