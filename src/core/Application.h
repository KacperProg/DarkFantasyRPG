#pragma once

#include <optional>

#include "camera/GameCamera.h"
#include "entities/Player.h"
#include "input/InputSystem.h"
#include "rendering/Renderer.h"
#include "world/World.h"

namespace core {

    // Application owns the window and the top-level game loop. It knows about
    // every subsystem so it can wire them together each frame, but it does not
    // contain gameplay rules itself - those live in world::World and
    // entities::Player.
    class Application {
    public:
        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        // Runs the game loop until the window should close. Blocking call.
        void Run();

    private:
        void Update(float deltaTime);
        void Render();

        input::InputSystem input_;
        world::World world_;
        entities::Player player_;
        camera::GameCamera camera_;

        // Renderer touches OpenGL (via raylib) on construction (it allocates a
        // RenderTexture2D), so it must not be built until AFTER InitWindow()
        // has created a valid GL context. A plain member would be constructed
        // during Application's member-initializer list - before InitWindow()
        // runs in the constructor body - so this is deliberately deferred and
        // constructed explicitly in Application::Application()'s body instead.
        std::optional<rendering::Renderer> renderer_;
    };

} // namespace core