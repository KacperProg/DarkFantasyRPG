#pragma once

#include "raylib.h" // for Vector2 - a plain POD struct, safe to expose here

namespace input {

// Remappable key bindings. Passed into InputSystem so control schemes can
// be swapped without touching gameplay code.
struct KeyBindings {
    int moveUp = 0;
    int moveDown = 0;
    int moveLeft = 0;
    int moveRight = 0;
    int jump = 0;
    int dodge = 0;
    int melee = 0;
    int ranged = 0;
    int debugToggle = 0;

    static KeyBindings Defaults();
};

// InputSystem is the single place gameplay code goes to ask "what is the
// player doing right now?". It knows about raylib key codes internally;
// nothing outside this class (and its .cpp) does.
//
// This intentionally does not know about players, cameras, or gameplay
// state - it only reports raw player intent for the current frame.
class InputSystem {
public:
    explicit InputSystem(KeyBindings bindings = KeyBindings::Defaults());

    // Call once per frame, before querying state.
    void Update();

    // Normalized movement direction on the world plane (X, Y). Zero vector
    // if no movement keys are held.
    Vector2 MoveDirection() const;

    bool IsRunHeld() const;

    // Edge-triggered: true only on the frame the key was pressed.
    bool IsJumpPressed() const;
    bool IsDodgePressed() const;
    bool IsMeleePressed() const;
    bool IsRangedPressed() const;
    bool IsDebugTogglePressed() const;

    const KeyBindings& Bindings() const { return bindings_; }
    void SetBindings(const KeyBindings& bindings) { bindings_ = bindings; }

private:
    KeyBindings bindings_;
};

} // namespace input
