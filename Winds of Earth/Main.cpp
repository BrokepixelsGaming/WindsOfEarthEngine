#pragma once

//#include <BleachLua/LuaState.h>

#include "RenderSystem\GameWindow.h"
#include "tools\BleachNew.h"
#include "EngineInterface\EngineSingleton.h"
#include "EngineInterface/Singleton.h"
#include "../Winds of Earth/source/Components/GameComponentFactory.h"

// includes based on debug
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

int main([[maybe_unused]]int argc, [[maybe_unused]] char* argv[])
{
	INIT_LEAK_DETECTOR();
	//_CrtSetBreakAlloc(11236); way to set a breakpoint to find memory leaks leaving here for example. 
	
	
	EngineSingleton::Create();
	GameComponentFactory gameComponentFactory;
	EngineSingleton::Get()->SetGameComponentFactory(&gameComponentFactory);

	if (!EngineSingleton::Get()->Init())
	{
#ifdef _DEBUG
		std::cout << "Engine startup failed.\n";
#endif // _DEBUG
		EngineSingleton::Get()->CleanUp();
		EngineSingleton::Destroy();
		DESTROY_LEAK_DETECTOR();
		return 1;
	}
	EngineSingleton::Get()->IdleEngine();
	EngineSingleton::Get()->CleanUp();
	EngineSingleton::Destroy();
	DESTROY_LEAK_DETECTOR();
	return 0;
}