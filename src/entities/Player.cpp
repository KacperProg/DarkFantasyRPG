#include "entities/Player.h"

#include <algorithm>
#include <cmath>

#include "config/GameConfig.h"
#include "input/InputSystem.h"
#include "world/World.h"

namespace entities {

Player::Player(float spawnX, float spawnY) : x_(spawnX), y_(spawnY) {}

float Player::CurrentMoveSpeed(const input::InputSystem& input) const {
    float speed = config::PlayerMoveSpeed;
    if (input.IsRunHeld()) {
        speed *= config::PlayerRunMultiplier;
    }
    return speed;
}

void Player::UpdateFacing(float moveX, float moveY) {
    if (moveX == 0.0f && moveY == 0.0f) {
        return; // keep last facing when idle
    }
    // Pick the dominant axis so facing reads clearly in a three-quarter
    // camera even when moving diagonally.
    if (std::fabs(moveX) > std::fabs(moveY)) {
        facing_ = moveX > 0.0f ? FacingDirection::Right : FacingDirection::Left;
    } else {
        facing_ = moveY > 0.0f ? FacingDirection::Down : FacingDirection::Up;
    }
}

void Player::UpdateJump(float deltaTime, const input::InputSystem& input) {
    if (IsGrounded() && input.IsJumpPressed()) {
        verticalVelocity_ = config::JumpInitialVelocity;
    }

    if (!IsGrounded() || verticalVelocity_ > 0.0f) {
        verticalVelocity_ -= config::Gravity * deltaTime;
        verticalVelocity_ = std::max(verticalVelocity_, -config::MaxFallSpeed);
        height_ += verticalVelocity_ * deltaTime;

        if (height_ <= 0.0f) {
            height_ = 0.0f;
            verticalVelocity_ = 0.0f;
        }
    }
}

void Player::UpdateDodgeTimers(float deltaTime) {
    if (dodgeTimeRemaining_ > 0.0f) {
        dodgeTimeRemaining_ = std::max(0.0f, dodgeTimeRemaining_ - deltaTime);
    }
    if (invulnerabilityTimeRemaining_ > 0.0f) {
        invulnerabilityTimeRemaining_ = std::max(0.0f, invulnerabilityTimeRemaining_ - deltaTime);
    }
    if (dodgeCooldownRemaining_ > 0.0f) {
        dodgeCooldownRemaining_ = std::max(0.0f, dodgeCooldownRemaining_ - deltaTime);
    }
}

void Player::Update(float deltaTime, const input::InputSystem& input, const world::World& world) {
    UpdateDodgeTimers(deltaTime);
    UpdateJump(deltaTime, input);

    // --- Movement input ---
    const auto moveDir = input.MoveDirection();
    float moveX = moveDir.x;
    float moveY = moveDir.y;

    bool canStartDodge = dodgeCooldownRemaining_ <= 0.0f && dodgeTimeRemaining_ <= 0.0f;
    if (input.IsDodgePressed() && canStartDodge) {
        // Dodge in the current movement direction; if the player isn't
        // moving, dodge in the direction they're currently facing.
        if (moveX != 0.0f || moveY != 0.0f) {
            float length = std::sqrt(moveX * moveX + moveY * moveY);
            dodgeDirectionX_ = moveX / length;
            dodgeDirectionY_ = moveY / length;
        } else {
            switch (facing_) {
                case FacingDirection::Up:    dodgeDirectionX_ = 0.0f;  dodgeDirectionY_ = -1.0f; break;
                case FacingDirection::Down:  dodgeDirectionX_ = 0.0f;  dodgeDirectionY_ = 1.0f;  break;
                case FacingDirection::Left:  dodgeDirectionX_ = -1.0f; dodgeDirectionY_ = 0.0f;  break;
                case FacingDirection::Right: dodgeDirectionX_ = 1.0f;  dodgeDirectionY_ = 0.0f;  break;
            }
        }
        dodgeTimeRemaining_ = config::DodgeDuration;
        dodgeCooldownRemaining_ = config::DodgeCooldown;
        invulnerabilityTimeRemaining_ = config::DodgeInvulnerabilityTime;
    }

    float desiredX = x_;
    float desiredY = y_;

    if (IsDodging()) {
        desiredX += dodgeDirectionX_ * config::DodgeSpeed * deltaTime;
        desiredY += dodgeDirectionY_ * config::DodgeSpeed * deltaTime;
        velocityX_ = dodgeDirectionX_ * config::DodgeSpeed;
        velocityY_ = dodgeDirectionY_ * config::DodgeSpeed;
    } else {
        float speed = CurrentMoveSpeed(input);
        desiredX += moveX * speed * deltaTime;
        desiredY += moveY * speed * deltaTime;
        velocityX_ = moveX * speed;
        velocityY_ = moveY * speed;
        UpdateFacing(moveX, moveY);
    }

    const auto resolved = world.ResolveCollisions(
        x_, y_, desiredX, desiredY,
        config::PlayerWidthUnits, config::PlayerDepthUnits);

    x_ = resolved.x;
    y_ = resolved.y;
}

} // namespace entities
