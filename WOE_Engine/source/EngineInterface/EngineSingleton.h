#pragma once

//#include <bleachlua/luastate.h>
//#include <bleachlua/luavar.h>
//#include <bleachlua/luafunction.h>

#include "..\RenderSystem\GameWindow.h"
#include "..\EventSystem\EventManager.h"
#include "WorldSystem/WorldManager.h"
#include "ResourceSystem/ResourceManager.h"

#include "..\tools\BleachNew.h"
#include "EngineInterface/Singleton.h"
#include "InputSystem/Keyboard.h"
#include "../../Winds of Earth/source/Components/GameComponentFactory.h"
#include "../RenderSystem/Camera.h"


class TileMap;

class EngineSingleton : public Singleton<EngineSingleton>
{
private:
	GameWindow* m_pGameWindow; // only 1.
	EventManager* m_pEventManager; // Main event Manager handles everything from world to inputs. 
	WorldManager* m_pWorldManager;
	ResourceManager* m_pResourceManager;
	//BleachLua::LuaState m_luaState;
	Keyboard m_pKeyboard;
	GameComponentFactory* m_pGameComponentFactory;
	Camera m_camera;

	void BindCPPFunctionsToEngineTable();

public:
	EngineSingleton();
	// Here is where we spin up all our managers.
	bool Init();
	void IdleEngine();
	void CleanUp();

	void SetGameComponentFactory(GameComponentFactory* factory) { m_pGameComponentFactory = factory; }
	GameComponentFactory* GetGameComponentFactory() { return m_pGameComponentFactory; }
	EventManager* GetEventManager() { return m_pEventManager; }
	//BleachLua::LuaState& GetLuaState() { return m_luaState; }
	ResourceManager* GetResourceManager() { return m_pResourceManager; }
	WorldManager* GetWorldManager() { return m_pWorldManager; }
	Keyboard& GetKeyboard() { return m_pKeyboard; }
	GameWindow* GetWindow() { return m_pGameWindow; }
	Camera& GetCamera() { return m_camera; }
};