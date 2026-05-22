# Shape Collector — AGENTS.md

## Overview
Simple 2D FreeGLUT/OpenGL game in C++. The player (triangle) catches falling shapes using WASD/arrows. No build system or tests exist.

## Project structure (flat)
```
main.cpp          — Entrypoint, GLUT setup, main loop
game.h / .cpp     — Game state machine (START_SCREEN → PLAYING → GAME_OVER), collision, spawning
renderer.h / .cpp — All OpenGL drawing (primitives, HUD, screens)
input.h / .cpp    — Keyboard + mouse callbacks
```

- `game.h` exposes globals (`gameState`, `player`, `items[]`, `spawnTimer`) used across all translation units
- `renderer.h` includes `game.h` (needs `Collectible` type)

## Build
No build system. Compile manually (requires FreeGLUT installed):
```
g++ main.cpp game.cpp renderer.cpp input.cpp -lfreeglut -lopengl32 -lglu32 -o shape_collector
```
On MSVC:
```
cl main.cpp game.cpp renderer.cpp input.cpp /link freeglut.lib opengl32.lib glu32.lib
```

## Windows notes
- Uses `sprintf_s` (MSVC runtime) in `renderer.cpp` — if using MinGW, replace with `sprintf`
- Uses `<GL/freeglut.h>` — requires FreeGLUT binaries on `PATH`

## No tests, no lint, no typecheck, no CI
Do not invent test/lint commands — the project has none.

## Game constants (defined in game.h)
| Constant | Value | Purpose |
|---|---|---|
| `MAX_ITEMS` | 13 | Max collectibles on screen |
| `BOUNDARY` | 0.85 | Player movement limit |
| `PLAYER_STEP` | 0.032 | Movement per keypress |
| `SPAWN_RATE` | 110 | Frames between spawns |
