# Roadmap

This is a living document - expect it to change as each milestone reveals
what the next one should actually be.

## Milestone 0 - Foundation (this commit)

- [x] CMake + raylib project scaffolding
- [x] Window, game loop, low-res pixel-perfect render target
- [x] Input abstraction with configurable bindings
- [x] Placeholder ruined-castle room with static obstacles
- [x] Continuous player movement + AABB collision
- [x] Explicit jump (height + gravity), not sprite-scaling
- [x] Dodge with cooldown and i-frames
- [x] Following camera with bounds and smoothing
- [x] Debug overlay (F1)

## Milestone 1 - Combat foundation

- [ ] Attack definition struct (range, damage, cooldown, type)
- [ ] Nearest-enemy targeting system, independent of attack implementations
- [ ] Melee attack (Z) with range check, damage, hit feedback
- [ ] Ranged attack (X): simple projectile (thrown knife or bolt), travel,
      impact detection, damage
- [ ] A single simple enemy: patrol or idle, health, can be targeted and
      killed
- [ ] Target indicator (visual)

## Milestone 2 - Tutorial integration

- [ ] Lightweight tutorial-state tracker (not hardcoded into gameplay classes)
- [ ] Movement / jump / dodge / melee / ranged tutorial beats triggered by
      actual player behaviour
- [ ] Minimal on-screen prompts

## Milestone 3 - Vertical traversal depth

- [ ] Height-aware collision (jumping over low hazards)
- [ ] Second room + a zone transition

## Later (not scheduled - see spec section 36)

Inventory, equipment, magic/abilities, character progression, NPCs,
dialogue, quests, shops, crafting, multiple enemy types, bosses, more
sophisticated AI, save/load, multiple zones, cinematics, status effects,
narrative systems.

None of these should be started before the milestone ahead of them is
actually working and playable.
