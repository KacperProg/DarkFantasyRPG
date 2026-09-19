#include "core/Application.h"

#include "config/GameConfig.h"
#include "raylib.h"

namespace core {

    Application::Application()
        : player_(world_.PlayerSpawn().x, world_.PlayerSpawn().y) {
        InitWindow(config::WindowWidth, config::WindowHeight, config::WindowTitle);
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        SetTargetFPS(60);

        // Must come after InitWindow(): Renderer allocates a RenderTexture2D,
        // which needs a live GL context. See the comment on renderer_'s
        // declaration in Application.h.
        renderer_.emplace();

        camera_.SetBounds(world_.Bounds());
    }

    Application::~Application() {
        CloseWindow();
    }

    void Application::Run() {
        while (!WindowShouldClose()) {
            float deltaTime = GetFrameTime();
            Update(deltaTime);
            Render();
        }
    }

    void Application::Update(float deltaTime) {
        input_.Update();

        if (input_.IsDebugTogglePressed()) {
            renderer_->SetDebugOverlayEnabled(!renderer_->IsDebugOverlayEnabled());
        }

        player_.Update(deltaTime, input_, world_);
        camera_.Update(deltaTime, player_.PositionX(), player_.PositionY(), player_.Height());
    }

    void Application::Render() {
        renderer_->BeginFrame(camera_);
        renderer_->DrawWorld(world_);
        renderer_->DrawPlayer(player_);
        renderer_->EndWorldSpace();
        renderer_->DrawDebugOverlay(player_, camera_, world_);
        renderer_->EndFrame();

        BeginDrawing();
        renderer_->PresentToScreen();
        EndDrawing();
    }

} // namespace core