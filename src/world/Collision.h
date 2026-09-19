#pragma once

namespace world {

// Axis-aligned bounding box on the world plane (X = horizontal, Y = depth).
// Deliberately just data + a couple of pure functions - no physics engine,
// no velocity resolution beyond simple axis-separated push-out, per the
// project's "lightweight collision, not a physics engine" requirement.
struct AABB {
    float x = 0.0f; // min corner
    float y = 0.0f; // min corner
    float width = 0.0f;
    float height = 0.0f;

    float MinX() const { return x; }
    float MinY() const { return y; }
    float MaxX() const { return x + width; }
    float MaxY() const { return y + height; }
};

inline bool Intersects(const AABB& a, const AABB& b) {
    return a.MinX() < b.MaxX() && a.MaxX() > b.MinX() &&
           a.MinY() < b.MaxY() && a.MaxY() > b.MinY();
}

// Returns a box centred at (centerX, centerY) with the given full width/height.
inline AABB MakeCenteredAABB(float centerX, float centerY, float width, float height) {
    return AABB{centerX - width * 0.5f, centerY - height * 0.5f, width, height};
}

} // namespace world
