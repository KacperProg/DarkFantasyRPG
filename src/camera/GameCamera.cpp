#include "camera/GameCamera.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "config/GameConfig.h"

namespace camera {

GameCamera::GameCamera() {
    camera_.offset = Vector2{config::RenderWidth / 2.0f, config::RenderHeight / 2.0f};
    camera_.target = Vector2{0.0f, 0.0f};
    camera_.rotation = 0.0f;
    camera_.zoom = config::CameraZoom;
}

void GameCamera::Shake(float intensity, float duration) {
    shakeIntensity_ = intensity;
    shakeTimeRemaining_ = duration;
}

void GameCamera::Update(float deltaTime, float targetX, float targetY, float targetHeight) {
    // Convert world-plane units to the internal render resolution's pixel
    // space. Jump height nudges the camera target upward slightly so
    // jumps have a small, readable visual effect without being jarring.
    float desiredX = (targetX + followOffsetX_) * config::PixelsPerUnit;
    float desiredY = (targetY + followOffsetY_) * config::PixelsPerUnit
                      - targetHeight * config::PixelsPerUnit * config::CameraHeightInfluence;

    if (hasBounds_) {
        float minX = bounds_.MinX() * config::PixelsPerUnit;
        float maxX = bounds_.MaxX() * config::PixelsPerUnit;
        float minY = bounds_.MinY() * config::PixelsPerUnit;
        float maxY = bounds_.MaxY() * config::PixelsPerUnit;
        desiredX = std::clamp(desiredX, minX, maxX);
        desiredY = std::clamp(desiredY, minY, maxY);
    }

    // Exponential smoothing toward the desired position. Deliberately not
    // over-smoothed - a high follow-speed constant keeps the camera from
    // feeling disconnected from player input, per the project's camera
    // requirements.
    float smoothing = 1.0f - std::exp(-config::CameraFollowSmoothing * deltaTime);
    currentX_ += (desiredX - currentX_) * smoothing;
    currentY_ += (desiredY - currentY_) * smoothing;

    float shakeX = 0.0f;
    float shakeY = 0.0f;
    if (shakeTimeRemaining_ > 0.0f) {
        shakeTimeRemaining_ = std::max(0.0f, shakeTimeRemaining_ - deltaTime);
        float falloff = shakeTimeRemaining_;
        shakeX = ((static_cast<float>(std::rand()) / RAND_MAX) - 0.5f) * 2.0f * shakeIntensity_ * falloff;
        shakeY = ((static_cast<float>(std::rand()) / RAND_MAX) - 0.5f) * 2.0f * shakeIntensity_ * falloff;
    }

    camera_.target = Vector2{currentX_ + shakeX, currentY_ + shakeY};
}

} // namespace camera
