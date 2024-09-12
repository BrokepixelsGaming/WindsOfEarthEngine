#pragma once

#include "ComponentSystem/Component.h"
#include "EntitySystem/Entity.h"
#include "tools/Utilities/XorShift.h"
#include "MapGeneratorComponent.h"
#include "tools/Utilities/Color.h"
#include <map>
#include "TileMapSystem/TileTypes.h"

using ValueMap = std::vector<std::vector<double>>;
static constexpr int kWorldSize = 500 * 500;

class WorldGenerationManager : 
	public Component
{
protected:
	StringId m_type = "WorldGenerationManager";
	Entity* m_pOwner;

	XorShift m_rng;
	MapGeneratorComponent m_baseMapGenerator;

	ValueMap m_heatMap;
	ValueMap m_heightMap;
	SDL_Surface* m_surface;
	SDL_Texture* m_worldTexture;
	
	SDL_FRect m_worldTextureTransform = {50.0f, 50.0f, 500.0f, 500.0f};
	Color m_pixelsInSurface[kWorldSize];
	Vector2<float> m_selectorLocation = { 0.0f, 0.0f};
	bool m_activeSelection = false;

	CardnalDirection m_direction;

public:
	WorldGenerationManager(Entity* pOwner);
	~WorldGenerationManager();

	void Update([[maybe_unused]] double deltaTimeMs) override;
	void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset) override;
	bool ProcessEvent(Event& evt) override;

private:
	void GenerateGlobalHeatMap();
	void GenerateGlobalHeightMap();
	void GenerateGlobalMapView();
	void GenerateTileMap();
	void ProcessTileMap(std::map<Color, float>& selectionData);
	void PrintColorToConsole(Color color);
	TileType ColorToTileType(Color color);

	// Helper
	bool PlaceTileOverlay(StringId& type);
	void PlaceObjectBasedOnTileTypeAtLocation(StringId& type, int x, int y);

	void DigA3x3CaveRoom(Vec2 locationOfCenterTile);
	void DigUntilEdgeOrNewBiome(Vec2 location);
	void DigTile(Vec2 location, TileType type);
	void RandomlyChangeDirections();
};