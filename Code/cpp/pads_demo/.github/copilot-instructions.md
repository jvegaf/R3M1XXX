# Copilot Project Instructions

Purpose: Guide AI agents contributing to this Raspberry Pi Pico (RP2040) PlatformIO project (Arduino framework) so they act productively without re-discovering basics.

## Project Shape
- Single PlatformIO environment defined in `platformio.ini` (`[env:pico]` using `platform = raspberrypi`, `board = pico`, `framework = arduino`). Add new environments (e.g. debug, release, pico_w) by copying this block.
- Source layout (standard PlatformIO skeleton):
  - `src/` (currently empty) — put all application `.cpp` / `.ino` / `.c` files here. Prefer `.cpp` for C++ features.
  - `include/` — public headers (`.h/.hpp`). Keep interfaces here to speed incremental builds.
  - `lib/<LibName>/src` — private component libraries (each self‑contained). Use when code can be reused or logically separated.
  - `test/` — unit tests (Unity framework via PlatformIO) when added.
  - `.github/` — automation & these instructions.

## Conventions
- Use Arduino core APIs for hardware abstractions unless a low-level SDK call (Pico SDK) is needed; isolate non‑Arduino RP2040 SDK calls behind thin wrappers inside `lib/`.
- Keep timing‑sensitive code (PIO, tight loops) separated into a library (e.g. `lib/Timing/`) so the main `loop()` stays readable.
- Prefer non‑blocking patterns (state machines, millis() deltas) instead of `delay()` in core logic; if a blocking wait is unavoidable, document why.
- File naming: `PascalCase` for library directories, `snake_case` for source files if not part of Arduino sketch entry (`main.cpp`). Headers mirror source names.
- Expose library APIs with minimal Arduino dependencies in headers; include Arduino headers only in `.cpp` where possible.

## Build / Run
- Core build: `pio run` (all envs) or `pio run -e pico`.
- Upload (USB): `pio run -e pico -t upload` (ensure Pico in BOOTSEL if needed or UF2 drag‑drop). Agents should not assume an attached device—guard upload steps behind explicit user intent.
- Clean: `pio run -t clean`.
- Add dependency: declare in a future `lib_deps` line inside the `[env:pico]` section (create it if absent) to ensure reproducible builds.

## Adding Code
1. Create or update `src/main.cpp` with standard Arduino entry points:
   ```cpp
   #include <Arduino.h>
   void setup() { /* init peripherals */ }
   void loop()  { /* non-blocking main logic */ }
   ```
2. If logic grows, move cohesive parts into `lib/MyFeature/src/` with a header in same folder; include via `#include <MyFeature.h>`.
3. Keep hardware pin assignments in a single header (propose `include/pins.h`) to avoid scattering magic numbers.
4. For compile-time constants, use `constexpr` or `enum class` in headers; avoid macros unless required.

## Testing (When Introduced)
- Place tests in `test/<name>/test_main.cpp` using Unity. Run: `pio test -e pico`.
- Mock hardware-dependent behavior by abstracting via interfaces in `include/` and providing stub implementations in tests.

## Performance / Resource Awareness
- RP2040 has dual cores; initial contributions should stay single-core unless concurrency is explicitly requested. If adding multicore code, encapsulate core1 entry in a function and start via `multicore_launch_core1()` inside a guarded section.
- Avoid dynamic allocation in loops; preallocate buffers or use static storage.

## Safe Changes for Agents
Good first contributions an agent can make without physical hardware confirmation:
- Introduce `src/main.cpp` scaffold if missing.
- Add `include/pins.h` centralizing (currently hypothetical) pin definitions.
- Refactor monolithic logic into a library under `lib/`.
- Add unit test scaffolding in `test/`.
- Add `lib_deps` for widely used Arduino-compatible libraries (only after checking they support RP2040 Arduino core).

## What NOT to Assume
- No existing business logic: don't fabricate features (keep additions minimal + documented rationale in PR).
- No network stack (WiFi/Bluetooth) since board is plain Pico (not Pico W); guard any such additions behind a new environment if requested.
- No custom build flags; propose them (e.g., `-DDEBUG=1`) only with justification.

## PR / Commit Style
- Small, cohesive commits; reference why (e.g., "Extract timing state machine to lib/Timing").
- Keep formatting consistent with existing files (currently un-opinionated; default PlatformIO). Avoid wholesale reformatting.

## Future Enhancements (Only on Request)
- Introduce separate `[env:pico_debug]` with `build_flags = -DDEBUG=1`.
- Setup GitHub Actions for CI (`pio run`, optional `pio test`).

---
Questions or unclear patterns: list them at the bottom of your PR description under "Open Questions" for human review.
