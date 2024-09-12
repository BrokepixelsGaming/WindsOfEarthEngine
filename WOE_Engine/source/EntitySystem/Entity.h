#pragma once

#include <vector>

#include "EventSystem/Event.h"
#include "ComponentSystem/Component.h"
#include "config/Settings.h"
#include "..\3rdParty\include\SDL\SDL.h"
#include "tools/Utilities/Vector.h"
#include "tools/BleachNew.h"
#include "tools/Utilities/StringId.h"


class Entity
{
public:
	static inline EntityID s_nextId = 1;  // 0 is an "invalid" id. There should never be an entity with a 0 for an ID
	
protected:
	EntityID m_id;
	Vec2 m_currentLocation;
	std::vector<Component*> m_components;
	
public:
	Entity(Vec2 location);
	~Entity();
	Entity(const Entity&) = default;
	Entity(Entity&&) = delete;
	Entity& operator=(const Entity&) = default;
	Entity& operator=(Entity&&) = delete;

	EntityID GetID() { return m_id; }
	bool ProcessEvent(Event& evt);
	void Update(double deltaTime);
	void Render(SDL_Renderer* pRenderer, Vec2 cameraOffset);
	void AddComponent(Component* component);
	void UpdateLocation(Vec2 location);
	const Vec2& GetLocation() { return m_currentLocation; }
	void SetLocation(Vec2 location);
	template <class Type>
	Type* GetComponent(StringId id);
};

template <class Type>
Type* Entity::GetComponent(StringId id)
{
	for (auto& component : m_components)
	{
		//const char* test = component->GetType();
		if (component->GetType() == id)
		{
			return static_cast<Type*>(component);
		}
	}
	return nullptr;
}