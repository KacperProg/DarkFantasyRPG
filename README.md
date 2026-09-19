# Dark Fantasy RPG - Vertical Slice

A dark, atmospheric 2.5D fantasy action RPG. Pixel-art presentation, an
angled three-quarter camera, continuous movement, and real-time combat
(coming after this milestone).

This repository currently contains the **movement/camera/collision
milestone**: a ruined castle room, continuous player movement, jump,
dodge, static-obstacle collision, and a following camera. Combat,
enemies, and the tutorial framework are the next milestones - see
`ROADMAP.md`.

## Prerequisites

- **Visual Studio 2022** with the "Desktop development with C++" workload
- **CMake 3.21+** (bundled with Visual Studio, or install separately)
- **Git**
- Internet access on first configure (CMake fetches raylib automatically)

## Build

From the repository root:

```
cmake -S . -B build
cmake --build build --config Debug
```

Or open the folder directly in Visual Studio ("Open a local folder") -
Visual Studio's built-in CMake support will detect `CMakeLists.txt`
automatically and configure it for you.

The first configure will take a little longer than usual while CMake
downloads and builds raylib.

## Run

```
build\Debug\DarkFantasyRPG.exe
```

Or press **F5** in Visual Studio with `DarkFantasyRPG` set as the startup
project (CMake sets this automatically).

Run the executable from the repository root (or via Visual Studio, which
sets the working directory automatically) so relative asset paths resolve
correctly once assets are added.

## Controls

| Action        | Key         |
|---------------|-------------|
| Move          | Arrow Keys  |
| Run           | Left Ctrl (hold) |
| Jump          | Space       |
| Dodge         | Left Shift  |
| Melee         | Z (reserved, not yet implemented) |
| Ranged        | X (reserved, not yet implemented) |
| Toggle debug overlay | F1  |

All bindings are configurable via `input::KeyBindings` - see
`src/input/InputSystem.h`.

## Project layout

See `ARCHITECTURE.md` for the full breakdown. Summary:

```
src/
  core/       Application lifecycle and the main loop
  config/     Central tuning constants
  input/      Input abstraction (keyboard -> gameplay intent)
  camera/     Camera following/bounds/shake
  world/      Room geometry and collision
  entities/   Player (and future enemies)
  rendering/  Everything that decides how state is drawn
```

## Known limitations (this milestone)

- No combat, enemies, or targeting yet.
- No animation - the player is a placeholder rounded rectangle.
- Single static room; no zone transitions yet.
- No audio.
- Jump has no interaction with obstacle height (you can't yet jump *over*
  a hazard - collision doesn't account for `Height()` yet). This is the
  natural next increment once combat lands, since it also matters for
  ranged-attack arcs.

## Development workflow

This project is intended to be developed incrementally through an
ongoing Claude Code (or manual) session. See `DECISIONS.md` for why key
technical choices were made, and `ROADMAP.md` for what's next.
