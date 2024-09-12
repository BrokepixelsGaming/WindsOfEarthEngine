#pragma once

#include <SDL/SDL.h>

// A SDL class to handle the interfacing and control the pointers. 
class GameWindow
{
private:
    // Pointer to the application window managed by SDL.
    SDL_Window* m_pWindow;
    // Pointer to the renderer that can be used to draw output.
    SDL_Renderer* m_pRenderer;
public:

    // Default Constructor
    GameWindow();
    ~GameWindow();

    SDL_Renderer* GetRenderer() { return m_pRenderer; }

    // Function to clean up SDL.
    void WindowCleanup();

    // May add parameters at a later date to allows custom windows, but for now just a preset to ensure stablility. Tied into the constructor. 
    bool RunSDLInit();

    // Draw's output on window.
    void DisplayOutput();
};