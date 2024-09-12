#include "EngineSingleton.h"
#include <chrono>

#if _FPS_COUNTER_
   #include <iostream>
#endif

//using namespace BleachLua;

void EngineSingleton::BindCPPFunctionsToEngineTable()
{

    //if (!m_luaState.DoFile("Data/Scripts/EngineFunctions.lua"))
    //{
    //    m_luaState.DoString("print('Engine functions not loaded :: Check EngSingleton.Init()')");
    //}
}

EngineSingleton::EngineSingleton()
    : m_pGameWindow(BLEACH_NEW(GameWindow))
    , m_pEventManager(BLEACH_NEW(EventManager))
    , m_pWorldManager(BLEACH_NEW(WorldManager))
    , m_pResourceManager(BLEACH_NEW(ResourceManager))
 //   , m_luaState()
    , m_pKeyboard()
{
}

void EngineSingleton::IdleEngine()
{
    bool quit = false;
    bool sdlQuit = false;
    //m_luaState.DoString("print('Engine Start Idle')");
    // Calculate Delta time
    // Get current time to store for the previous loop. 
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    static int counter = 0;
    static double fpsTimer = 0;

    while (!quit && !sdlQuit)
    {
        // Get current frame time. 
        auto thisFrameTime = std::chrono::high_resolution_clock::now();

        // Get duration that has passed.
        std::chrono::duration<double> lastFrameDuration = thisFrameTime - lastFrameTime;

        // Convert duration into a double which we use for delta time. 
        double deltaTime = lastFrameDuration.count();

        // Reset lastFrameTime to the thisFrameTime.
        lastFrameTime = thisFrameTime;

        // Extract SDLEvents and take care of the high level ones while Qing inputs. 
        sdlQuit = m_pEventManager->ProcessAndQueueSDLEvents();
        quit = m_pEventManager->ProcessEvents();

        // if anythig is asking to quit we don't need to update the world.  
        if (quit || sdlQuit)
            break;
        //float tempDistance;
        // Update the camera
        m_camera.Update(deltaTime);
        //for (int i = 0; i < 50; ++i)
        //{
        //    for (int j = 0; j < 50; ++j)
        //    {
        //       tempDistance = (std::powf((i), 2) + std::powf((j), 2));
        //    }
        //}
        
        // Need to start looking at building world->shard->Entity
        m_pWorldManager->UpdateWorlds(deltaTime);

        // Write to the renderer the state of the world. 
        m_pWorldManager->DisplayActiveWorld(m_pGameWindow->GetRenderer());

        // Displays the renderer to the user. 
        m_pGameWindow->DisplayOutput();
        m_pKeyboard.Reset();
#if _FPS_COUNTER_ //FPS
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0)
        {
            std::cout << counter << " FPS\n";
            counter = 0;
            fpsTimer = 0;
        }
        else
        {
            ++counter;
        }
#endif // _DEBUG
    }    
}

///////////////////////////////////////////////////////////////
// Spin up of the engine and all of the managers. 
///////////////////////////////////////////////////////////////
bool EngineSingleton::Init()
{
    // RenderWindow
    if (!m_pGameWindow->RunSDLInit()) 
    {
        BLEACH_DELETE(m_pGameWindow);
        return false;
    }
    
    // spin up Event system
    if (!m_pEventManager->Init())
    {
        BLEACH_DELETE(m_pEventManager);
        return false;
    }
  
    // spin up Resource System
    if (!m_pResourceManager->Init())
    {
        BLEACH_DELETE(m_pResourceManager);
        return false;
    }

    //// spin up LuaState
    //if(!m_luaState.Init())
    //{
    //    return false;
    //}
    //// Set the default state
    //LuaVar::SetDefaultLuaState(&m_luaState);

    // Setting the Engine Table for Lua.
    //LuaVar engineTable;
    //engineTable.CreateNewTable();
    //m_luaState.SetGlobal("Engine", engineTable);

    // This is to make it easy and uniform as to where we bind the functions to Lua.
    //BindCPPFunctionsToEngineTable();
    //LuaFunction<void> PrintError = m_luaState.GetGlobal<LuaVar>("PrintError");

    //PrintError("This is an engine start up test.");
    
    // spin up World System
    if (!m_pWorldManager->LoadWorldsData())
    {
        BLEACH_DELETE(m_pWorldManager);
        return false;
    }

    return true;
}

void EngineSingleton::CleanUp()
{
    m_pResourceManager->FlushAllResources();
    BLEACH_DELETE(m_pGameWindow);
    BLEACH_DELETE(m_pWorldManager);
    BLEACH_DELETE(m_pResourceManager);
    BLEACH_DELETE(m_pEventManager);
}
