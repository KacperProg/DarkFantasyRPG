#include "rendering/Renderer.h"

#include <algorithm>
#include <cstdio>
#include <string>

#include "camera/GameCamera.h"
#include "config/GameConfig.h"
#include "entities/Player.h"
#include "world/World.h"

namespace rendering {

namespace {

Color ColorForObstacle(const std::string& label) {
    if (label == "rubble_hazard") return Color{120, 70, 50, 255};
    if (label == "fallen_pillar") return Color{90, 90, 100, 255};
    return Color{60, 58, 66, 255}; // broken_wall / default
}

Vector2 WorldToRenderPixels(float worldX, float worldY) {
    return Vector2{worldX * config::PixelsPerUnit, worldY * config::PixelsPerUnit};
}

} // namespace

Renderer::Renderer() {
    target_ = LoadRenderTexture(config::RenderWidth, config::RenderHeight);
    SetTextureFilter(target_.texture, TEXTURE_FILTER_POINT); // crisp pixel scaling, no smoothing
}

Renderer::~Renderer() {
    UnloadRenderTexture(target_);
}

void Renderer::BeginFrame(const camera::GameCamera& camera) {
    BeginTextureMode(target_);
    ClearBackground(Color{18, 16, 22, 255}); // near-black stone/void backdrop
    BeginMode2D(camera.RaylibCamera());
}

void Renderer::DrawWorld(const world::World& worldRef) {
    const auto& bounds = worldRef.Bounds();
    Vector2 floorTopLeft = WorldToRenderPixels(bounds.MinX(), bounds.MinY());
    Vector2 floorSize = WorldToRenderPixels(bounds.width, bounds.height);
    DrawRectangle(static_cast<int>(floorTopLeft.x), static_cast<int>(floorTopLeft.y),
                  static_cast<int>(floorSize.x), static_cast<int>(floorSize.y),
                  Color{38, 34, 40, 255}); // ruined stone floor placeholder
    DrawRectangleLinesEx(
        Rectangle{floorTopLeft.x, floorTopLeft.y, floorSize.x, floorSize.y},
        2.0f, Color{10, 9, 12, 255});

    for (const auto& obstacle : worldRef.Obstacles()) {
        Vector2 topLeft = WorldToRenderPixels(obstacle.bounds.MinX(), obstacle.bounds.MinY());
        Vector2 size = WorldToRenderPixels(obstacle.bounds.width, obstacle.bounds.height);
        DrawRectangle(static_cast<int>(topLeft.x), static_cast<int>(topLeft.y),
                      static_cast<int>(size.x), static_cast<int>(size.y),
                      ColorForObstacle(obstacle.label));
    }
}

void Renderer::DrawPlayer(const entities::Player& player) {
    Vector2 groundPos = WorldToRenderPixels(player.PositionX(), player.PositionY());
    float heightPixels = player.Height() * config::PixelsPerUnit;

    // Shadow stays on the ground plane regardless of jump height - this is
    // what sells the vertical displacement instead of just scaling a sprite.
    float shadowScale = 1.0f - std::min(player.Height() / 3.0f, 0.6f);
    DrawEllipse(static_cast<int>(groundPos.x), static_cast<int>(groundPos.y),
                8.0f * shadowScale, 4.0f * shadowScale, Color{0, 0, 0, 110});

    Color bodyColor = Color{200, 195, 180, 255};
    if (player.IsDodging()) {
        bodyColor = Color{160, 200, 230, 255};
    } else if (player.IsInvulnerable()) {
        bodyColor = Color{160, 200, 230, 160};
    }

    float spriteWidth = config::PlayerWidthUnits * config::PixelsPerUnit;
    float spriteHeight = 1.6f * config::PixelsPerUnit; // taller than footprint - a standing figure
    Rectangle spriteRect{
        groundPos.x - spriteWidth * 0.5f,
        groundPos.y - spriteHeight - heightPixels,
        spriteWidth,
        spriteHeight};
    DrawRectangleRounded(spriteRect, 0.15f, 4, bodyColor);

    // Small facing indicator so direction reads clearly with a placeholder box sprite.
    Vector2 facingDot = groundPos;
    const float dotOffset = 6.0f;
    switch (player.Facing()) {
        case entities::FacingDirection::Up:    facingDot.y -= dotOffset + heightPixels; break;
        case entities::FacingDirection::Down:  facingDot.y += dotOffset - heightPixels; break;
        case entities::FacingDirection::Left:  facingDot.x -= dotOffset; break;
        case entities::FacingDirection::Right: facingDot.x += dotOffset; break;
    }
    //DrawCircle(static_cast<int>(facingDot.x), static_cast<int>(facingDot.y - spriteHeight * 0.5f),
    //           2.0f, Color{40, 35, 45, 255});
}

void Renderer::EndWorldSpace() {
    EndMode2D();
}

void Renderer::DrawDebugOverlay(const entities::Player& player, const camera::GameCamera& camera,
                                 const world::World& worldRef) {
    (void)worldRef;
    if (!debugOverlayEnabled_) return;

    DrawFPS(4, 4);

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), "pos: (%.2f, %.2f)  h: %.2f", player.PositionX(),
                  player.PositionY(), player.Height());
    DrawText(buffer, 4, 20, 10, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "vel: (%.2f, %.2f)", player.VelocityX(), player.VelocityY());
    DrawText(buffer, 4, 32, 10, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "grounded: %s  dodging: %s  i-frames: %s",
                  player.IsGrounded() ? "yes" : "no",
                  player.IsDodging() ? "yes" : "no",
                  player.IsInvulnerable() ? "yes" : "no");
    DrawText(buffer, 4, 44, 10, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "dodge cooldown: %.2fs", player.DodgeCooldownRemaining());
    DrawText(buffer, 4, 56, 10, RAYWHITE);

    Vector2 camTarget = camera.RaylibCamera().target;
    std::snprintf(buffer, sizeof(buffer), "camera target (px): (%.1f, %.1f)  zoom: %.2f", camTarget.x,
                  camTarget.y, camera.RaylibCamera().zoom);
    DrawText(buffer, 4, 68, 10, RAYWHITE);
}

void Renderer::EndFrame() {
    EndTextureMode();
}

void Renderer::PresentToScreen() {
    float scaleX = static_cast<float>(GetScreenWidth()) / config::RenderWidth;
    float scaleY = static_cast<float>(GetScreenHeight()) / config::RenderHeight;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    float drawWidth = config::RenderWidth * scale;
    float drawHeight = config::RenderHeight * scale;
    float offsetX = (GetScreenWidth() - drawWidth) * 0.5f;
    float offsetY = (GetScreenHeight() - drawHeight) * 0.5f;

    Rectangle source{0.0f, 0.0f, static_cast<float>(target_.texture.width),
                      -static_cast<float>(target_.texture.height)}; // flip Y (render textures are upside down)
    Rectangle dest{offsetX, offsetY, drawWidth, drawHeight};

    ClearBackground(BLACK);
    DrawTexturePro(target_.texture, source, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

} // namespace rendering
