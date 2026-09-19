#pragma once

#include "raylib.h"
#include "world/Collision.h"

namespace camera {

// GameCamera owns all camera behaviour: following, smoothing, bounds
// clamping, and (later) shake/scripted movement. The player never touches
// the raylib Camera2D directly - it only exists here.
class GameCamera {
public:
    GameCamera();

    // targetX/targetY: world-plane point the camera should frame (usually
    // the player's position). targetHeight: current jump height, used to
    // give a subtle vertical shift so jumps read visually.
    void Update(float deltaTime, float targetX, float targetY, float targetHeight);

    void SetBounds(const world::AABB& bounds) { bounds_ = bounds; hasBounds_ = true; }
    void ClearBounds() { hasBounds_ = false; }

    void SetFollowOffset(float offsetX, float offsetY) { followOffsetX_ = offsetX; followOffsetY_ = offsetY; }

    // Triggers a short screen shake. Placeholder hook for combat feedback;
    // not used by the movement-only milestone but wired up so future
    // combat code has somewhere to call into.
    void Shake(float intensity, float duration);

    const Camera2D& RaylibCamera() const { return camera_; }

private:
    Camera2D camera_{};

    float currentX_ = 0.0f;
    float currentY_ = 0.0f;

    float followOffsetX_ = 0.0f;
    float followOffsetY_ = 0.0f;

    world::AABB bounds_{};
    bool hasBounds_ = false;

    float shakeTimeRemaining_ = 0.0f;
    float shakeIntensity_ = 0.0f;
};

} // namespace camera
