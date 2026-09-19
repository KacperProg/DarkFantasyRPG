#pragma once

// -----------------------------------------------------------------------
// GameConfig
//
// Central, plain-data home for gameplay/rendering tuning values.
// Nothing here depends on raylib so this header can be included by any
// gameplay system without pulling in rendering code.
//
// This is intentionally a flat namespace of constants for the prototype
// milestone. If/when attack definitions, enemy stat blocks, etc. arrive,
// they should get their own small data structures rather than growing
// this file indefinitely.
// -----------------------------------------------------------------------
namespace config {

// ---- Window / rendering -------------------------------------------------
inline constexpr int WindowWidth = 1280;
inline constexpr int WindowHeight = 720;
inline constexpr const char* WindowTitle = "Dark Fantasy RPG - Vertical Slice";

// The game renders to a small internal target at this resolution, then
// scales it up with nearest-neighbour filtering to fill the window.
// This is what keeps pixel art crisp regardless of window size.
inline constexpr int RenderWidth = 480;
inline constexpr int RenderHeight = 270;

// ---- World plane / player ------------------------------------------------
// World-plane coordinates (X = horizontal, Y = depth) are in "world units",
// not pixels. Rendering multiplies by PixelsPerUnit when drawing.
inline constexpr float PixelsPerUnit = 16.0f;

inline constexpr float PlayerWidthUnits = 0.8f;
inline constexpr float PlayerDepthUnits = 0.6f; // footprint depth, used for collision
inline constexpr float PlayerMoveSpeed = 3.6f;  // units/sec
inline constexpr float PlayerRunMultiplier = 1.6f;

// ---- Vertical displacement (jump) ---------------------------------------
inline constexpr float Gravity = 14.0f;          // units/sec^2 applied to vertical velocity
inline constexpr float JumpInitialVelocity = 5.2f; // units/sec, upward
inline constexpr float MaxFallSpeed = 12.0f;

// ---- Dodge ---------------------------------------------------------------
inline constexpr float DodgeSpeed = 9.0f;         // units/sec during the dodge burst
inline constexpr float DodgeDuration = 0.18f;     // seconds the burst lasts
inline constexpr float DodgeCooldown = 0.55f;     // seconds before dodge can be used again
inline constexpr float DodgeInvulnerabilityTime = 0.18f; // seconds of i-frames, <= DodgeDuration

// ---- Camera ---------------------------------------------------------------
inline constexpr float CameraFollowSmoothing = 8.0f; // higher = snappier follow
inline constexpr float CameraHeightInfluence = 0.5f; // how much player jump height shifts camera target
inline constexpr float CameraZoom = 3.0f;            // applied on top of the internal render resolution

} // namespace config
