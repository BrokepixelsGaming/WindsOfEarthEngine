#pragma once
#include <vector>

#include "config/Settings.h"
#include "EntitySystem/Entity.h"
#include "..\3rdParty\include\SDL\SDL.h"
#include "tools/Utilities/tinyxml2.h"

///////////////////////////////////////////////////////////////
// Entity will control the lifespans of object within the worlds and shards. 
///////////////////////////////////////////////////////////////
class EntityManager
{
	// do I have 1 more many. 
	std::vector<Entity*> m_entities;
	std::vector<Entity*> m_uiEntities;
public:
	EntityManager();
	~EntityManager();
	void UpdateEntities(double deltaTime);
	void Render(SDL_Renderer* pRenderer);
	Entity* CreateEntity(std::string command);
	Entity* CreateEntity(const tinyxml2::XMLElement* pEntites);
	// TODO: create entity Pair()
	void CreateEntityPair(const tinyxml2::XMLElement* pElementOfPair);
	void LoadEntityComponents(const tinyxml2::XMLElement* pComponents, Entity* pEntity);
	bool LoadEntities(const tinyxml2::XMLElement* pEntites);
	void DestroyEntity(EntityID id);
	const std::vector<Entity*> GetAllEntities()const { return m_entities; }
};