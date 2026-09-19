# Architecture

## Coordinate system

- **World plane**: `X` (horizontal) and `Y` (depth) in *world units*, not
  pixels. `config::PixelsPerUnit` converts world units to pixels in the
  internal render target.
- **Height (`H`)**: vertical displacement above the ground plane, used for
  jumping. It is tracked explicitly on the entity (`Player::Height()`)
  rather than faked by scaling a sprite. Visually, height shifts the
  sprite upward on screen (and, subtly, the camera target) while the
  shadow stays on the ground plane at the entity's `(X, Y)` - this is what
  sells the jump in a three-quarter camera.
- Collision is resolved entirely on the `X`/`Y` world plane. `H` does not
  currently affect collision (see `README.md` known limitations) - a
  hazard can't yet be jumped *over*, only walked around. That interaction
  is deferred until it's actually needed (the rubble hazard in the
  starting room is a placeholder for when it is).

## Rendering model

- The game renders to a small internal `RenderTexture2D` at
  `config::RenderWidth x RenderHeight` (480x270), using point/nearest
  filtering, then scales that texture up to fill the actual window
  (`Renderer::PresentToScreen`), preserving aspect ratio with letterboxing.
  This keeps pixel art crisp at any window size and decouples gameplay
  coordinates from screen pixels entirely - changing the window resolution
  never touches gameplay code.
- Layering within a frame: floor -> obstacles -> player -> (leave camera
  space) -> debug overlay. There's no z-sorting system yet since the
  vertical slice only has one dynamic entity; once enemies/projectiles
  exist, draw order should sort by world-plane `Y` (depth) so entities
  correctly occlude each other, similar to the reference presentation
  style. That's a `rendering::Renderer` concern only - gameplay code will
  not need to change.

## System responsibilities

| System | Owns | Does not know about |
|---|---|---|
| `input::InputSystem` | Raw key bindings, per-frame intent queries | Player, camera, world |
| `world::World` | Room bounds, static obstacles, collision resolution | Rendering, input, player |
| `entities::Player` | Position, velocity, height/jump state, dodge state machine | Rendering, raylib (beyond the `Vector2` value type returned by input) |
| `camera::GameCamera` | Follow smoothing, bounds clamping, shake | Player internals - it's fed plain `(x, y, height)` |
| `rendering::Renderer` | The low-res render target, all drawing, debug overlay | Gameplay rules |
| `core::Application` | Window lifecycle, per-frame wiring of the above | Gameplay rules, drawing details |

This separation is what lets, e.g., the render resolution or the camera's
smoothing curve change without touching `Player`, and lets `Player`'s
movement rules change without touching how it's drawn.

## Collision

`world::World::ResolveCollisions` is a simple axis-separated AABB
resolver: it tests the X-axis move first, then the Y-axis move using the
(possibly corrected) X position. This gives predictable "slide along the
wall" behaviour on diagonal movement without needing a physics engine.
Collision geometry (`world::AABB`) is entirely separate from render
geometry - `Renderer` reads obstacle bounds to draw them, but nothing
about drawing feeds back into collision.

## Why no ECS / physics engine / scripting system (yet)

See `DECISIONS.md`. In short: the current entity count and interaction
complexity don't justify the overhead. The architecture (composition over
inheritance, no owning raw pointers, clear system boundaries) doesn't
block introducing any of these later if a genuine need arises.

## Extending this milestone

- **New obstacle types**: add to `world::Obstacle` / `World::BuildRuinedCastleRoom`
  and a case in `rendering::ColorForObstacle`. No changes to `Player` needed.
- **New input actions**: add a binding to `input::KeyBindings`, a query
  method to `InputSystem`, wire it in `Player::Update` or `Application::Update`.
- **Enemies**: expected to live in `src/enemies/`, mirroring `Player`'s
  shape (position/velocity/state), consumed by a future `combat::TargetingSystem`
  that stays independent of both `Player` and enemy classes per the
  project's targeting requirement.
- **Attacks**: expected to live in `src/combat/`, as small data-driven
  attack definitions (range, damage, cooldown) rather than being embedded
  in `Player`.
