# Decisions

Significant architectural decisions and why they were made. Trivial
implementation details are not recorded here.

## raylib as the rendering/window/input/audio framework

Lightweight, C-based, minimal dependency footprint, well suited to a
solo/small-scale 2D pixel-art project, has first-class `RenderTexture2D`
support which is exactly what's needed for the pixel-perfect scaling
approach. Avoids the overhead of a full engine (Unreal/Unity/Godot) that
the project explicitly wants to avoid taking on.

## 2D gameplay simulation with 2.5D visual presentation

The gameplay plane is `(X, Y)` in world units; height (`H`) is a separate
explicit scalar for jump, not a third simulated spatial dimension. A true
3D simulation was rejected: nothing in the required feature set (movement,
jump, melee/ranged combat, hazards, traversal) needs full 3D physics or
navigation, and a 2D plane + explicit height scalar is dramatically
simpler to reason about, collide, and debug, while still visually
supporting jumping and future vertical geometry. This can be revisited if
a future requirement (e.g. multi-level verticality with overlapping
floors) genuinely demands it.

## No physics engine

Collision needs for this milestone are: static AABB obstacles, a moving
player AABB, simple gravity for jump height. A full physics engine (e.g.
Box2D) would add a dependency, a coordinate/units translation layer, and
solver behaviour (restitution, friction, contact resolution) that this
game does not need and that would need to be carefully suppressed to keep
the crisp, deterministic movement the game wants. A hand-written
axis-separated AABB resolver is simpler, fully deterministic, and easy to
extend later (capsules, triggers) without pulling in a general-purpose
solver.

## No ECS

Current entity count and behavioural complexity (one player, static
obstacles) does not justify an ECS's indirection cost. Plain classes with
composition (`Player` owns its own movement/jump/dodge state; `World`
owns obstacle data) are more readable at this scale. This should be
revisited if/when the enemy count and the combinatorics of
components/behaviours (many enemy types x many status effects x many
attack types) actually get large enough that inheritance/composition
starts producing duplication - not before.

## Input abstraction layer

`input::InputSystem` is the only place that calls raylib's
`IsKeyDown`/`IsKeyPressed`. This was introduced now (rather than deferred)
because the project explicitly requires configurable bindings and future
controller support, and retrofitting an abstraction after gameplay code
has scattered raw key checks through it is far more error-prone than
introducing the seam up front. The cost of this abstraction today is a
handful of methods on a small class - low enough to be worth paying
immediately.

## Camera as a dedicated, player-independent system

`camera::GameCamera` is fed plain `(x, y, height)` values rather than a
reference to `Player`. This keeps the camera reusable for any future
followable target (e.g. a cinematic focus point) and keeps `Player`
completely unaware that a camera exists, matching the project's
requirement that the player not directly manipulate the rendering camera.

## Low-resolution render target for pixel scaling

Chosen over rendering directly at window resolution with scaled sprites,
because a single internal target guarantees every pixel-art element scales
by the same integer-ish factor and stays aligned, and it makes gameplay
coordinates (world units) fully independent of window size by
construction - resizing the window only affects `Renderer::PresentToScreen`.
