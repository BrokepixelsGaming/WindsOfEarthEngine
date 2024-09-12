#pragma once
#include "ComponentSystem/Component.h"
#include "EntitySystem/Entity.h"
#include "tools/Utilities/XorShift.h"

class MapGeneratorComponent :
    public Component
{
protected:
	StringId m_type = "MapGeneratorComponent";
	Entity* m_pOwner;

	bool m_hasRun = false;
	XorShift m_rng;

public:
	MapGeneratorComponent(Entity* owner);
	void Update([[maybe_unused]]double deltaTimeMs) override;
	void GenerateHeightMapUsingNoise(unsigned int seed = 0);
private:

	float NormalizeNoise(unsigned int noise);
	//float GetNoise(int worldX, int worldY, int maxWorldX, int maxWorldY, int noiseInputRange);
};
