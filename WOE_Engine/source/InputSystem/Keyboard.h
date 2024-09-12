#pragma once

#include <cinttypes>
#include <array>

#include "../InputSystem/CodeLog.h"



class Keyboard
{
public:
    // A log containing the last 10 key presses and will check to run a code. 
    CodeLog codeLog;

    Keyboard();
    /// The allowed values for KeyCode align with Windows' Virtual Key defines.
    /// See https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    /// Despite being an int, the maximum allowed value is 255
    using KeyCode = uint32_t;

    /// Returns whether the key represented by the given code is currently depressed.
    bool IsKeyDown(KeyCode keyCode) const;

    /// Returns whether the key represented by the given code was pressed and then released.
    /// This implies there is a one-frame lag
    bool WasKeyPressed(KeyCode keyCode) const;

    /// Marks whether the key represented by the given code is currently depressed.
    void SetKeyState(KeyCode keyCode, bool isDown);

    /// Sets the state of all keys back to unpressed and mark keys that were previously depressed.
    /// This is to be called at the end of a frame.
    void Reset();


private:
    std::array<bool, 256> m_KeyState;
    std::array<bool, 256> m_pressedKeys;
};

// automaton keys
constexpr KeyCode kRKey = 114; // Reset to empty for automaton
constexpr KeyCode kFKey = 102; // Seed for automaton
constexpr KeyCode kVKey = 118; // One Step of the rules
constexpr KeyCode kTKey = 116; // Run in Loop
constexpr KeyCode kGKey = 103; // increase size of neightborhood
constexpr KeyCode kBKey = 98; // Decrease size of neightborhood
// Map Keys
constexpr KeyCode kYKey = 121; // reset
constexpr KeyCode kHKey = 104; // seed for map generation
constexpr KeyCode kNKey = 110; // run rules for map
constexpr KeyCode kUKey = 117; // clean up edges
constexpr KeyCode kJKey = 106; // run a preset pattern
constexpr KeyCode kShiftKey = 106; // run a preset pattern