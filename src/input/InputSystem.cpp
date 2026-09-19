#include "input/InputSystem.h"

#include <cmath>

#include "raylib.h"

namespace input {

KeyBindings KeyBindings::Defaults() {
    KeyBindings bindings;
    bindings.moveUp = KEY_UP;
    bindings.moveDown = KEY_DOWN;
    bindings.moveLeft = KEY_LEFT;
    bindings.moveRight = KEY_RIGHT;
    bindings.jump = KEY_SPACE;
    bindings.dodge = KEY_LEFT_SHIFT;
    bindings.melee = KEY_Z;
    bindings.ranged = KEY_X;
    bindings.debugToggle = KEY_F1;
    return bindings;
}

InputSystem::InputSystem(KeyBindings bindings) : bindings_(bindings) {}

void InputSystem::Update() {
    // Reserved for future needs (e.g. gamepad polling, buffered input).
    // raylib already tracks pressed/down state per-frame internally, so
    // there is nothing to cache today, but keeping Update() here means
    // callers don't need to change when that becomes necessary.
}

Vector2 InputSystem::MoveDirection() const {
    float x = 0.0f;
    float y = 0.0f;

    if (IsKeyDown(bindings_.moveLeft)) x -= 1.0f;
    if (IsKeyDown(bindings_.moveRight)) x += 1.0f;
    if (IsKeyDown(bindings_.moveUp)) y -= 1.0f;
    if (IsKeyDown(bindings_.moveDown)) y += 1.0f;

    Vector2 dir{x, y};
    float lengthSq = dir.x * dir.x + dir.y * dir.y;
    if (lengthSq > 1.0f) {
        float length = std::sqrt(lengthSq);
        dir.x /= length;
        dir.y /= length;
    }
    return dir;
}

bool InputSystem::IsRunHeld() const {
    return IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
}

bool InputSystem::IsJumpPressed() const { return IsKeyPressed(bindings_.jump); }
bool InputSystem::IsDodgePressed() const { return IsKeyPressed(bindings_.dodge); }
bool InputSystem::IsMeleePressed() const { return IsKeyPressed(bindings_.melee); }
bool InputSystem::IsRangedPressed() const { return IsKeyPressed(bindings_.ranged); }
bool InputSystem::IsDebugTogglePressed() const { return IsKeyPressed(bindings_.debugToggle); }

} // namespace input
