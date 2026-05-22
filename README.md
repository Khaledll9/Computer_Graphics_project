
<!-- ============================================================ -->
<!--  SHAPE COLLECTOR — README                                     -->
<!--  Generated from full source analysis of game.h, game.cpp,      -->
<!--  renderer.h, renderer.cpp, input.h, input.cpp, main.cpp       -->
<!-- ============================================================ -->

<br />
<p align="center">
  <img src="https://img.shields.io/badge/version-1.0.0-blue?style=flat-square" alt="Version" />
  <img src="https://img.shields.io/badge/license-MIT-green?style=flat-square" alt="License" />
  <img src="https://img.shields.io/badge/OpenGL-4.6-orange?style=flat-square&logo=opengl" alt="OpenGL" />
  <img src="https://img.shields.io/badge/C%2B%2B-C%2B%2B11-00599C?style=flat-square&logo=cplusplus" alt="C++11" />
  <img src="https://img.shields.io/badge/FreeGLUT-3.4.0-ff69b4?style=flat-square" alt="FreeGLUT" />
</p>

<h1 align="center">🎮 Shape Collector</h1>

<p align="center">
  <em>A retro-style 2D arcade game built with OpenGL and FreeGLUT — catch falling shapes, dodge missed ones, and survive as long as you can.</em>
</p>

<br />

---

## 📖 About The Project

**Shape Collector** is a lightweight desktop arcade game written in **C++** using the **FreeGLUT** library (a free, open-source alternative to GLUT). The player controls a blue triangle at the bottom of the screen and must catch falling geometric shapes — golden balls (worth +10) and cyan polygons (worth +5). Miss a shape and you lose a life; lose all three and the game is over.

This project was built as a hands-on exploration of:
- Immediate-mode OpenGL rendering (`glBegin`/`glEnd`)
- Frame-based game loops and timer callbacks
- State machine design (`START_SCREEN → PLAYING → GAME_OVER`)
- Simple circle-circle collision detection
- Keyboard & mouse event handling via GLUT callbacks

**Who is this for?**
- Students learning OpenGL / Computer Graphics fundamentals
- Developers interested in retro game loop architecture
- Anyone looking for a compact, well-separated C++ graphics project to study or extend

---

## ✨ Key Features

| Feature | Details |
|---|---|
| 🎯 **Three-state game flow** | Start screen → Playing → Game Over with smooth transitions |
| 🟡 **Two collectible types** | Golden balls (+10 pts) and cyan polygons (+5 pts) with randomized drop speeds |
| 🎨 **Interactive color picker** | Click anywhere to cycle the player triangle through random colors |
| ⌨️ **Dual input support** | WASD keys **and** arrow keys for movement; Space to start/restart |
| ❤️ **Life system** | 3 lives per round; missed shapes cost one life each |
| 📊 **Real-time HUD** | Live score display + remaining life indicators rendered as miniature triangles |
| 🖥️ **Border-constrained playfield** | Player movement clamped to ±0.85 units for clean screen margins |
| ⚡ **Double-buffered rendering** | `GLUT_DOUBLE` for flicker-free 60 FPS animation |

---

## 🏗️ Architecture & Tech Stack

### Tech Stack

| Layer | Technology |
|---|---|
| **Language** | C++11 |
| **Graphics API** | OpenGL 1.x (immediate mode via FreeGLUT) |
| **Windowing & Input** | FreeGLUT 3.4+ |
| **Math** | `<math.h>` — circle vertex generation |
| **Build** | Manual `g++` or MSVC `cl` (no build system) |

### Project Architecture

The code follows a **single-directory separation of concerns** with four logical modules:

```
┌───────────────────────────────────────────────────┐
│                    main.cpp                        │
│  (entrypoint, GLUT setup, timer loop)             │
└───────┬───────────┬───────────┬───────────────────┘
        │           │           │
   ┌────▼────┐ ┌────▼────┐ ┌───▼──────┐
   │ game.h  │ │renderer │ │ input.h  │
   │game.cpp │ │ .h/.cpp │ │input.cpp │
   └─────────┘ └─────────┘ └──────────┘
```

| Module | Responsibility |
|---|---|
| **`main.cpp`** | GLUT initialization, window creation, callback registration, main loop |
| **`game.h` / `game.cpp`** | Global state (`GameState`, `Player`, `Collectible[]`), spawning, physics, collision detection |
| **`renderer.h` / `renderer.cpp`** | All OpenGL drawing primitives, HUD, start/game-over screens |
| **`input.h` / `input.cpp`** | Keyboard (regular + special) and mouse event handlers |

### State Machine

```
┌──────────────┐    Space    ┌──────────┐   lives == 0   ┌────────────┐
│ START_SCREEN │ ──────────► │ PLAYING  │ ──────────────► │ GAME_OVER  │
└──────────────┘             └──────────┘                 └────────────┘
                                  ▲                             │
                                  └──── Space + resetGame() ────┘
```

---

## 🚀 Getting Started

### Prerequisites

- **Windows** or **Linux** (tested on Windows 11, should work on any platform with FreeGLUT)
- A C++ compiler: **MinGW g++** or **MSVC cl.exe**
- **FreeGLUT** runtime & development libraries installed and discoverable on your system `PATH`

### Installation

1. **Clone the repository**

   ```bash
   git clone https://github.com/your-username/shape-collector.git
   cd shape-collector
   ```

2. **Install FreeGLUT**

   - **Windows (MinGW):** Download pre-built binaries from [freeglut.sourceforge.net](https://freeglut.sourceforge.net/), place `freeglut.dll` in your `PATH` (or alongside the executable), and ensure `libfreeglut.a` is reachable by the linker.
   - **Windows (MSVC):** Install via vcpkg: `vcpkg install freeglut` or download the official MSVC binaries.
   - **Linux (Debian/Ubuntu):** `sudo apt install freeglut3-dev`
   - **macOS:** `brew install freeglut`

3. **Compile**

   **MinGW (g++):**
   ```bash
   g++ main.cpp game.cpp renderer.cpp input.cpp \
       -lfreeglut -lopengl32 -lglu32 \
       -o shape_collector.exe
   ```

   **MSVC (cl):**
   ```bash
   cl main.cpp game.cpp renderer.cpp input.cpp \
       /link freeglut.lib opengl32.lib glu32.lib
   ```

4. **Run**

   ```bash
   ./shape_collector.exe        # Windows
   ./shape_collector            # Linux / macOS
   ```

> **Note for MinGW users:** `renderer.cpp` uses `sprintf_s` (MSVC-specific). If your MinGW build fails, replace `sprintf_s` with `sprintf` in `renderer.cpp:86,143`.

---

## 📸 Screenshots

<!-- TODO: Add actual screenshots here -->
| Start Screen | Gameplay | Game Over |
|---|---|---|
| `screenshot_start.png` | `screenshot_playing.png` | `screenshot_gameover.png` |

*A short screen recording or GIF would also be great — try [OBS Studio](https://obsproject.com/) or [ScreenToGif](https://www.screentogif.com/).*

---

## 🎯 Controls

| Key | Action |
|---|---|
| `W` / `↑` | Move up |
| `A` / `←` | Move left |
| `S` / `↓` | Move down |
| `D` / `→` | Move right |
| `Space` | Start game / Restart after game over |
| `Esc` | Quit the application |
| **Mouse click** | Change player color randomly |

---

## 🧪 Project File Map

```
my_project/
├── main.cpp          # Entrypoint, GLUT init, display & update loop
├── game.h            # Structs (Player, Collectible), enums, constants, extern globals
├── game.cpp          # Game logic: spawn, update, collision, state machine
├── renderer.h        # Drawing function declarations
├── renderer.cpp      # OpenGL rendering: primitives, HUD, screens
├── input.h           # Input callback declarations
├── input.cpp         # Keyboard & mouse event handlers
├── AGENTS.md         # AI assistant instructions for OpenCode sessions
└── README.md         # This file
```

---

## 🤝 Contact & Support

**Maintainer:** Khaled  
**Email:** *(add your email)*  
**LinkedIn:** *(add your LinkedIn URL)*  
**GitHub Issues:** [https://github.com/Khaled119/shape-collector/issues](https://github.com/Khaled119/shape-collector/issues)

Contributions, issues, and feature requests are welcome!  
Feel free to check the [issues page](../../issues) or open a pull request.

---

<p align="center">
  Made with ❤️ and C++ · OpenGL · FreeGLUT
</p>
```
