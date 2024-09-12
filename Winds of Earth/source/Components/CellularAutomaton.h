#pragma once
#include "ComponentSystem\Component.h"

#include "EntitySystem/Entity.h"
#include "tools/Utilities/XorShift.h"
#include "TileMapSystem/TileTypes.h"
#include "../../WOE_Engine/source/tools/Utilities/Vector.h"

using FakeMap = std::vector<std::pair<StringId, StringId>>;

class CellularAutomaton :
    public Component
{
protected:
	StringId m_type = "CellularAutomaton";
	Entity* m_pOwner;
	bool m_freeRun = false;
	double m_timer = 0.0;

	XorShift m_rng;

public:
	CellularAutomaton(Entity* owner);
	void Update([[maybe_unused]] double deltaTimeMs) override;

private:
	void RunRuleSet();
	void SaltWorld();
	int CountTypeInNeightborhood(FakeMap& map, Vector2<int> CurrentCell, TileType type,  int sizeOfNeighborhood);
	void ResetMapForGeneration();
	void SeedMapForGeneration();
	void GenerateMapDefault();
	void RunMapRules();
};

