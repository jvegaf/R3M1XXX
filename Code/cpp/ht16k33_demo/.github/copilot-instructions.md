# Copilot Instructions for ht16k33_demo (PlatformIO C++ Project)

## Project Overview
- This is a PlatformIO-based C++ project targeting microcontrollers (e.g., Raspberry Pi Pico).
- Main application code is in `src/main.cpp`.
- Project structure follows PlatformIO conventions: `src/`, `include/`, `lib/`, and `test/`.

## Architecture & Code Organization
- **src/**: Main source files. Entry point is typically `main.cpp`.
- **include/**: Shared header files for project-wide declarations and macros. Use `#include "header.h"` for headers here.
- **lib/**: Private libraries. Each library should be in its own subdirectory (e.g., `lib/Foo/`). PlatformIO will build and link these automatically.
- **test/**: Unit tests for PlatformIO Test Runner. See PlatformIO docs for test conventions.

## Build, Test, and Debug Workflows
- **Build:** Use PlatformIO commands (`pio run`) or VS Code PlatformIO extension to build firmware.
- **Upload:** Use `pio run --target upload` to flash firmware to device.
- **Test:** Place test code in `test/`. Run tests with `pio test` or via PlatformIO Test Runner in VS Code.
- **Debug:** Debugging is configured via `.vscode/launch.json` using PlatformIO's debug adapter. Use the "PIO Debug" launch configuration.

## Key Conventions & Patterns
- **Header Files:** Place shared headers in `include/`. Name with `.h` extension. Include in source files as `#include "header.h"`.
- **Libraries:** Organize custom libraries in `lib/your_library_name/`. Each should have its own `src/` and optional `library.json` for custom build options.
- **Tests:** Follow PlatformIO's unit testing conventions. See `test/README` and PlatformIO docs for details.
- **External Dependencies:** Managed via `platformio.ini`. Add libraries or frameworks here.

## Integration Points
- **PlatformIO:** All build, upload, test, and debug operations are managed by PlatformIO. Do not use raw `make` or other build systems.
- **Toolchain:** Uses GCC ARM toolchain for Raspberry Pi Pico (see `.vscode/launch.json`).
- **Framework:** Project may use Arduino-mbed or other PlatformIO-supported frameworks as specified in `platformio.ini`.

## Examples
- To include a header from `include/`, use: `#include "myheader.h"`
- To use a library from `lib/Foo/`, include its header: `#include <Foo.h>`
- To run unit tests: `pio test`

## References
- [PlatformIO Project Structure](https://docs.platformio.org/en/latest/projectconf/index.html)
- [PlatformIO Unit Testing](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)
- [PlatformIO Debugging](https://docs.platformio.org/en/latest/plus/debugging.html)

---

**If any conventions or workflows are unclear, please provide feedback so this guide can be improved.**
