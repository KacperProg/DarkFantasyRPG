#include "world/World.h"

namespace world {

namespace {

// Tests a proposed centred box against the room bounds and every
// obstacle. Returns true if the box is fully valid (inside bounds and
// not overlapping any obstacle).
bool IsPositionValid(const AABB& box, const AABB& bounds, const std::vector<Obstacle>& obstacles) {
    if (box.MinX() < bounds.MinX() || box.MaxX() > bounds.MaxX() ||
        box.MinY() < bounds.MinY() || box.MaxY() > bounds.MaxY()) {
        return false;
    }
    for (const auto& obstacle : obstacles) {
        if (Intersects(box, obstacle.bounds)) {
            return false;
        }
    }
    return true;
}

} // namespace

World::World() {
    BuildRuinedCastleRoom();
}

void World::BuildRuinedCastleRoom() {
    // A single ruined great-hall style room. Units are world units, not
    // pixels - see config::PixelsPerUnit for the conversion used at
    // render time. This is placeholder geometry meant to exercise
    // collision, camera framing and the tutorial route; it is not final
    // level art.
    bounds_ = AABB{0.0f, 0.0f, 26.0f, 16.0f};

    playerSpawn_ = SpawnPoint{3.0f, 8.0f};

    obstacles_.clear();

    // Broken outer wall stubs jutting into the room.
    obstacles_.push_back({AABB{6.0f, 0.0f, 1.0f, 4.0f}, "broken_wall"});
    obstacles_.push_back({AABB{6.0f, 12.0f, 1.0f, 4.0f}, "broken_wall"});

    // A collapsed pillar blocking a direct line through the hall -
    // encourages the player to move around it, useful for the movement
    // tutorial beat.
    obstacles_.push_back({AABB{12.0f, 6.5f, 1.4f, 1.4f}, "fallen_pillar"});

    // A rubble pile intended as the first jump-over hazard.
    obstacles_.push_back({AABB{16.0f, 7.0f, 1.8f, 2.0f}, "rubble_hazard"});

    // Narrow gap formed by two wall stubs, good for the dodge tutorial
    // (dash through/along it under time pressure later).
    obstacles_.push_back({AABB{20.0f, 0.0f, 1.0f, 6.0f}, "broken_wall"});
    obstacles_.push_back({AABB{20.0f, 10.0f, 1.0f, 6.0f}, "broken_wall"});
}

SpawnPoint World::ResolveCollisions(float prevX, float prevY,
                                     float desiredX, float desiredY,
                                     float footprintWidth, float footprintHeight) const {
    float resolvedX = prevX;
    float resolvedY = prevY;

    // Resolve X axis first.
    AABB testBoxX = MakeCenteredAABB(desiredX, resolvedY, footprintWidth, footprintHeight);
    if (IsPositionValid(testBoxX, bounds_, obstacles_)) {
        resolvedX = desiredX;
    }

    // Then resolve Y axis using the (possibly corrected) X position.
    // Testing axes independently is what gives predictable "slide along
    // the wall" behaviour instead of the player sticking in place when
    // moving diagonally into a corner.
    AABB testBoxY = MakeCenteredAABB(resolvedX, desiredY, footprintWidth, footprintHeight);
    if (IsPositionValid(testBoxY, bounds_, obstacles_)) {
        resolvedY = desiredY;
    }

    return SpawnPoint{resolvedX, resolvedY};
}

} // namespace world
