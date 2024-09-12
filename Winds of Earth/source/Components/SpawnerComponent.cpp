#include "SpawnerComponent.h"
#include "EngineInterface/EngineSingleton.h"
#include "EntitySystem/Entity.h"
#include "RenderSystem/Camera.h"

SpawnerComponent::SpawnerComponent(std::string entityToSpawn)
	: m_entityToSpawn(entityToSpawn)
{
}

SpawnerComponent::~SpawnerComponent()
{
}

void SpawnerComponent::SpawnEntity(int x, int y)
{
	Entity* pPointer = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->CreateEntity(m_entityToSpawn);

	auto& camera = EngineSingleton::Get()->GetCamera();
	if (pPointer != nullptr)
	{
		World* pWorld = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld();
		float snapX = static_cast<float>(pWorld->SnapX(camera.GetOffsetX() + x));
		float snapY = static_cast<float>(pWorld->SnapY(camera.GetOffsetY() + y));

		pPointer->SetLocation(Vec2(snapX, snapY));
		//pPointer->SetLocation(Vec2((x), (y))); sub class: SpawnUnit
	}
	//pPointer->SetLocation(Vec2(x, y));
	//EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()
}
