#pragma once

#include <string>
#include <vector>

#include "world/Collision.h"

namespace world {

// A single static piece of collidable geometry (a wall segment, a rubble
// pile, a broken pillar, ...). Kept as plain data so the collision system
// stays simple and debuggable.
struct Obstacle {
    AABB bounds;
    std::string label; // for debug overlay / future differentiation (hazard vs wall)
};

struct SpawnPoint {
    float x = 0.0f;
    float y = 0.0f;
};

// World is intentionally a single "room" for the vertical slice. It is
// structured so that later this becomes one Zone among many rather than
// requiring a rewrite: Zone/World-graph, multiple rooms, and transitions
// can be layered on top of this without changing how collision or
// rendering consume it.
class World {
public:
    World();

    const std::vector<Obstacle>& Obstacles() const { return obstacles_; }
    const AABB& Bounds() const { return bounds_; }
    const SpawnPoint& PlayerSpawn() const { return playerSpawn_; }

    // Resolves movement of an entity from (prevX, prevY) to the desired
    // (desiredX, desiredY), given its footprint size, by testing the X and
    // Y axes separately against all static obstacles and the room bounds.
    // Returns the corrected centre position the entity should end up at.
    // This is a simple, deterministic push-out resolver - not a physics
    // simulation - which keeps sliding-along-walls behaviour predictable.
    SpawnPoint ResolveCollisions(float prevX, float prevY,
                                  float desiredX, float desiredY,
                                  float footprintWidth, float footprintHeight) const;

private:
    void BuildRuinedCastleRoom();

    AABB bounds_;
    std::vector<Obstacle> obstacles_;
    SpawnPoint playerSpawn_;
};

} // namespace world
