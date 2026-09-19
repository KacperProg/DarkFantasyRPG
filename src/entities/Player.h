#pragma once

namespace world {
class World;
}

namespace input {
class InputSystem;
}

namespace entities {

enum class FacingDirection {
    Up,
    Down,
    Left,
    Right,
};

// Player holds only the state needed for the current milestone: movement,
// jumping, and dodging. Combat-related fields (health, current target)
// will be added when the combat milestone starts, rather than being
// stubbed out unused ahead of time.
class Player {
public:
    Player(float spawnX, float spawnY);

    void Update(float deltaTime, const input::InputSystem& input, const world::World& world);

    // World-plane position (X = horizontal, Y = depth).
    float PositionX() const { return x_; }
    float PositionY() const { return y_; }

    // Vertical displacement above the ground plane, used for jump.
    float Height() const { return height_; }
    bool IsGrounded() const { return height_ <= 0.0f; }

    FacingDirection Facing() const { return facing_; }

    bool IsDodging() const { return dodgeTimeRemaining_ > 0.0f; }
    bool IsInvulnerable() const { return invulnerabilityTimeRemaining_ > 0.0f; }
    float DodgeCooldownRemaining() const { return dodgeCooldownRemaining_ > 0.0f ? dodgeCooldownRemaining_ : 0.0f; }

    // Velocity in units/sec on the world plane - exposed for the debug
    // overlay, not intended to be mutated externally.
    float VelocityX() const { return velocityX_; }
    float VelocityY() const { return velocityY_; }

private:
    void UpdateFacing(float moveX, float moveY);
    void UpdateJump(float deltaTime, const input::InputSystem& input);
    void UpdateDodgeTimers(float deltaTime);
    float CurrentMoveSpeed(const input::InputSystem& input) const;

    float x_;
    float y_;

    float velocityX_ = 0.0f;
    float velocityY_ = 0.0f;

    float height_ = 0.0f;
    float verticalVelocity_ = 0.0f;

    FacingDirection facing_ = FacingDirection::Down;

    // Dodge state machine. All three are simple countdown timers rather
    // than an enum + timer pair, since the transitions are strictly
    // sequential (trigger -> active -> cooldown).
    float dodgeTimeRemaining_ = 0.0f;
    float dodgeCooldownRemaining_ = 0.0f;
    float invulnerabilityTimeRemaining_ = 0.0f;
    float dodgeDirectionX_ = 0.0f;
    float dodgeDirectionY_ = -1.0f;
};

} // namespace entities
