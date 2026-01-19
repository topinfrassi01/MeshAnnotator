# MeshAnnotator — Development Setup

This repository is configured for developing with C++11 using CMake and vcpkg, and provides basic VS Code integration.

Quick start

1. Install vcpkg (https://github.com/microsoft/vcpkg) and either:
   - set `VCPKG_ROOT` env var to vcpkg root, or
   - place vcpkg checkout in the project at `./vcpkg`.

2. Install dependencies (manifest mode):
   - From the project root run: `vcpkg install --triplet x64-windows` (or let CMake handle it during configure)

3. Configure & build:
   - Using VS Code: use the CMake extension or run `Tasks: Run Task -> CMake: Configure` then `CMake: Build`.
   - Or on the command line:
     ```bash
     cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
     cmake --build build --config Debug
     ctest --test-dir build --output-on-failure -C Debug
     ```

Notes

- The top-level `CMakeLists.txt` will attempt to detect a vcpkg toolchain automatically when `CMAKE_TOOLCHAIN_FILE` isn't passed in.
- Use the `CMake: Build` task as the pre-launch task for debugging.

## VS Code usage

- Install the **CMake Tools** extension (ms-vscode.cmake-tools) and the **C/C++** extension for full editor and debugging support.
- Open the project folder in VS Code. Use the **Command Palette** (Ctrl+Shift+P) and run `CMake: Configure` to generate the build files, then `CMake: Build` to compile.
- Use the provided Tasks from `Tasks -> Run Task`:
  - `CMake: Configure` — configure the project (choose build type)
  - `CMake: Build` — build the active configuration
  - `CTest: Run` — run unit tests
- To debug, either set the active target via the CMake extension or use the provided launch configuration **Launch with CMake (Windows)** and start debugging (F5).
- If vcpkg is present in the repo (./vcpkg) or `VCPKG_ROOT` is set, the toolchain will be detected automatically and packages from `vcpkg.json` will be used.

If you'd like, I can also add a short `.vscode` README snippet or a sample launch configuration for other platforms (gdb/lldb).