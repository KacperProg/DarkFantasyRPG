#pragma once

#include "raylib.h"

namespace world {
class World;
}
namespace entities {
class Player;
}
namespace camera {
class GameCamera;
}

namespace rendering {

// Renderer owns the low-resolution render target used for crisp pixel-art
// scaling, and all drawing. Gameplay code never issues raylib draw calls
// directly - it hands Renderer world/entity state and Renderer decides how
// it appears (layering, colours, debug overlays, etc.).
class Renderer {
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Begins the low-res render target and enters world-space (camera)
    // drawing mode. DrawWorld()/DrawPlayer() must be called between this
    // and EndWorldSpace().
    void BeginFrame(const camera::GameCamera& camera);
    void DrawWorld(const world::World& world);
    void DrawPlayer(const entities::Player& player);

    // Leaves world-space (camera) mode. Screen-space overlays (debug HUD)
    // must be drawn after this, before EndFrame().
    void EndWorldSpace();
    void EndFrame();

    // Presents the scaled low-res target to the actual window. Call after
    // EndFrame(), outside of BeginDrawing/EndDrawing for the target itself
    // but inside the window's own BeginDrawing/EndDrawing pair.
    void PresentToScreen();

    void SetDebugOverlayEnabled(bool enabled) { debugOverlayEnabled_ = enabled; }
    bool IsDebugOverlayEnabled() const { return debugOverlayEnabled_; }

    void DrawDebugOverlay(const entities::Player& player, const camera::GameCamera& camera,
                           const world::World& world);

private:
    RenderTexture2D target_{};
    bool debugOverlayEnabled_ = false;
};

} // namespace rendering
