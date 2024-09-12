
#include <iostream>

#include <SDL\SDL_ttf.h>
#include <SDL\SDL_mixer.h>

#include "GameWindow.h"
#include "tools\BleachNew.h"
#include "config\Settings.h"

GameWindow::GameWindow()
    : m_pRenderer(nullptr)
    , m_pWindow(nullptr)
{
}

// Default constructor.
GameWindow::~GameWindow()
{
    WindowCleanup();
}

///////////////////////////////////////////////////////////////
// Function to clean up SDL.
///////////////////////////////////////////////////////////////
void GameWindow::WindowCleanup()
{
    Mix_CloseAudio();
    TTF_Quit();
    // Destroy Renderer.
    SDL_DestroyRenderer(m_pRenderer);

    // SDL window cleanup.
    SDL_DestroyWindow(m_pWindow);

    SDL_Quit();
}
///////////////////////////////////////////////////////////////////////////////
// May add parameters at a later date to allows custom windows, 
// but for now just a preset to ensure stablility. Tied into the constructor. 
///////////////////////////////////////////////////////////////////////////////
bool GameWindow::RunSDLInit()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        // On Failure prints message and prints error string. 
        std::cout << "Failed to initialize SDL. Error: " << SDL_GetError() << std::endl;
        WindowCleanup();
        return false;
    }
    else
    {
        // On success, prints message.
        std::cout << "Successfully initialized SDL!" << std::endl;
    }

    // SDL window init.
    m_pWindow = SDL_CreateWindow(kGameNameString, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kMaxWindowWidth, kMaxWindowHeight, 0);

    if (m_pWindow == nullptr)
    {
        std::cout << "failed to create window. Error:" << SDL_GetError();
        WindowCleanup();
        return false;
    }

    // Create Renderer.
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
    if (m_pRenderer == nullptr)
    {
        std::cout << "failed to create renderer. Error:" << SDL_GetError();
        WindowCleanup();
        return false;
    }

    // Init TTF 
    if (TTF_Init() != 0)
    {
        std::cout << "failed to Init TTF. Error:" << SDL_GetError();
        WindowCleanup();
        return false;
    }
    // Initialize SDL_Mixer
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0)
    {
        std::cout << "failed to Init Audio. Error:" << SDL_GetError();
        WindowCleanup();
        return false;
    }

    // Init MP3 support
    int audioFlags = MIX_INIT_MP3;
    int errorCode = Mix_Init(audioFlags);
    if (errorCode != audioFlags)
    {
        std::cout << "Mixer_Init() failed. Error: " << Mix_GetError() << std::endl;
        WindowCleanup();
        return false;
    }
    return true;
}
void GameWindow::DisplayOutput()
{
    // Presenting. 
    SDL_RenderPresent(m_pRenderer);
    SDL_RenderClear(m_pRenderer);
}