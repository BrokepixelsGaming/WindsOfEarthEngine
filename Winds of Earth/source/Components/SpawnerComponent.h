#pragma once
#include "ComponentSystem/Component.h"

class SpawnerComponent : public Component
{
protected:
	StringId m_type = "Spawner";
	std::string m_entityToSpawn;
public:
	SpawnerComponent(std::string entityToSpawn);
	~SpawnerComponent();

	virtual StringId GetType() override { return m_type; }

	virtual void SpawnEntity(int x, int y);
	void SetEntityToSpawn(std::string entity) { m_entityToSpawn = entity; }
};