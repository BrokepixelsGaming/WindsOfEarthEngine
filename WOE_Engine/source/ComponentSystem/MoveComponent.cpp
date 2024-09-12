#include "MoveComponent.h"
#include "EventSystem/Event.h"
#include "config/Settings.h"
#include "EngineInterface/EngineSingleton.h"
//#include <BleachLua/LuaState.h>

MoveComponent::MoveComponent(Entity* owner)
	: m_pOwner(owner)
{
}

bool MoveComponent::ProcessEvent([[maybe_unused]]Event& evt)
{
	return false;
}

void MoveComponent::Update([[maybe_unused]] double deltaTime)
{
}
