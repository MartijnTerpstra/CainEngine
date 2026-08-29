# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

CainEngine is a C++23 game engine (Windows-first, with an in-progress Linux port) built from
scratch with a graphics-API-agnostic renderer. Currently only the DX11 backend is wired into the
build; DX12 and Vulkan backends exist as source but are commented out of the top-level CMake build
(`add_subdirectory(Graphics.DX12)` / `Graphics.Vulkan` are disabled in `CMakeLists.txt`).

## Build

CMake + Ninja (or Visual Studio) on Windows, CMake + Unix Makefiles on Linux. There is no build
script beyond CMake itself — configure then build:

```
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

`CMakeSettings.json` defines the Visual Studio "Open Folder" presets (`Windows-x64-Debug`,
`Windows-x64-Release`, `Linux-x64-Debug` via remote build). Release builds require MSVC
specifically — the top-level `CMakeLists.txt` hard-errors (`FATAL_ERROR`) for Clang/GCC/Intel
optimizer flags in non-Debug builds; only Debug configuration is currently portable across
compilers.

Third-party dependencies live under `Dependencies/` (`mst`, `Abseil`, `googletest`,
`nlohmann_json`) but are **not tracked by git** — each subfolder's `.gitignore` excludes
everything except its own `CMakeLists.txt`. Dependency source must already be present in these
folders (as vendored/copied code, not a git submodule) before configuring; there is no fetch
step in this repo.

### Tests

`BUILD_TESTING` (CMake option) enables `enable_testing()` and pulls in `googletest`. The engine's
own test target, `EntitySystem/Test`, is currently a stub CMakeLists (no tests wired up yet) —
don't assume `ctest` exercises engine code today. googletest is otherwise only exercised by the
vendored `mst` dependency's own test suites.

## Architecture

### Module layout: `External/` is the public API, the rest is private implementation

Every module (`Common`, `Platform`, `Graphics`, `EntitySystem`, `Engine`, `ResourceSystem`, the
`Graphics.*`/`Platform.*` backends...) follows the same shape:

- `<Module>/External/<Module>.h` — umbrella header for the module, included by consumers as
  `#include <ModuleName.h>` (e.g. `#include <Engine.h>`, `#include <Platform.Win32.h>`).
- `<Module>/External/<Module>/*.h` — the module's public interface headers. Only this directory
  is exposed to other modules (`target_include_directories(... PUBLIC/INTERFACE
  ${PROJECT_SOURCE_DIR}/External)`).
- `<Module>/*.cpp` (and sometimes `.h`) directly under the module root — private implementation,
  not visible to consumers.
- `<Module>/Precomp.h` / `Precomp.cpp` — precompiled header pair, wired up via the
  `add_precompiled_header` macro in `CMake/CMakeLists.txt`. Every module's `Precomp.h` just
  `#include`s its own `External/<Module>.h`, so the PCH is effectively "the whole public API is
  always available." New `.cpp` files need a matching precompile flag entry generated
  automatically by that macro — no manual per-file PCH wiring needed.

When adding a new public type to a module, put the declaration under `External/<Module>/` and the
implementation `.cpp` in the module root; when adding something module-internal, keep both in the
module root so it isn't exported.

### Dependency graph (mirrors `target_link_libraries` in each `CMakeLists.txt`)

```
Common  (foundation: logging, RefPtr/BaseObject casting, tasks, JSON parsing, Source/streams)
  ├── Platform            (interfaces: IWindow, IMonitor, IProcess, ICoreFactory, IInput...)
  │     └── Platform.Win32 / Platform.Xorg   (concrete implementations of Platform's interfaces)
  ├── EntitySystem         (Scene/EntityID/Transform — ECS-ish, not dependent on Platform/Graphics)
  └── Graphics             (interfaces: IRenderer, IFactory, IBuffer, ITexture... + Model/Material/ShaderManager)
        ├── Graphics.DX11 / .DX12 / .Vulkan   (concrete backend implementations)
        └── Engine                             (ties Scene + Renderer + CameraManager + ModelManager together)

Application.Win32 / Editor   -> link Common + Platform.Win32 + Graphics.DX11 (+ Engine for the app)
ResourceSystem                -> standalone, depends only on Common (not currently linked into the main build)
```

- **`Common`** is the base every module builds on: `Common::BaseObject`/`RefPtr<T>`/`CastHandle<T>`
  give an intrusive-refcounted, `dynamic_cast`-free "as/is" casting system (see
  `Common/External/Common/BaseObject.h`); `Common::Source`/`FileSource` is the binary-stream
  reader used for shader blobs and resources; `Common::TaskManager` provides the task system.
  `Common/External/Common.h` is included by every module's PCH and defines the vocabulary types
  used unqualified everywhere: `string`, `vector`, `RefPtr<T>`, `flag<T>`, `float2/3/4`,
  `matrix3x4`, `quaternion`, `uuid`, `stack_string<N>`, `inlined_vector`, `flat_hash_map`, etc.
  (aliased from `mst` and `Abseil`). Don't re-`#include <vector>`/qualify `std::` for these —
  follow the existing unqualified style.
- **Interface/implementation split**: `Platform` and `Graphics` define pure-virtual interfaces
  (`Platform::IWindow`, `Graphics::API::IRenderer`, `Graphics::API::IFactory`, ...) with no
  platform/API-specific code; `Platform.Win32`/`Platform.Xorg` and
  `Graphics.DX11`/`.DX12`/`.Vulkan` provide the concrete backend. `Graphics::Renderer` and
  `Graphics::Factory` (in `Graphics/`) are the API-agnostic wrapper types application code
  actually uses; a backend is selected by calling that backend's `CreateInstance()` (see
  `Application.Win32/main.cpp`: `Graphics::DX11::CreateInstance()`,
  `Platform::Win32::CreateInstance()`).
- **`EntitySystem`**: `Scene` owns a flat array of `EntityData` (fixed 128-byte layout,
  free-list–based reuse, `flat_hash_map`-based hierarchy/name/uuid lookups). `EntityID` is a
  handle into it. `Transform` / `CTransform` are thin mutable/const accessor views into a
  `Scene`'s entity data (`scene.GetTransform(entity)`), not separate storage — this
  mutable-view/const-view pair pattern (`X`/`CX`) recurs elsewhere in the codebase (e.g. renderer
  display settings) and is the idiom to follow for similar accessor types. `EntitySystem` has no
  dependency on `Graphics` or `Platform`.
- **`Engine`** is the composition root: owns the `Scene`, `Graphics::Renderer`,
  `Graphics::CameraManager`, `Graphics::ModelManager`, and `TaskManager`, and is a singleton
  reachable via `Engine::Get()`. `Application.Win32/main.cpp` is the best example of full
  end-to-end wiring (platform window creation → engine init → per-frame render/event loop).
- **Shaders**: HLSL sources live under `Content/Shaders source/`; `Editor.ShaderCompiler`
  (`DXCompiler`/`ICompiler`) compiles them offline into the binary blob format read at runtime —
  `Content/Shaders/DX11.shaders` / `DX12.shaders` (a `Common::Source`-readable container of
  compiled shader bytecode + reflection data), consumed by `Graphics::ShaderManager::Init`. The
  `Editor` target drives `Editor::ShaderSourceManager::CompileShaders()` to regenerate these.
- **`ResourceSystem`** (`ResourceManager`) exists as a module but is not currently added to the
  top-level `CMakeLists.txt` build (`Common`/`EntitySystem`/`Platform`/`Graphics`/`Content`/
  `Engine` are; `ResourceSystem` is not) — treat it as work-in-progress/not wired in.

### Conventions worth knowing

- Interfaces use `COMMON_DECLARE_INTERFACE(TypeName)` (protected ctor, pure-virtual-but-defined
  dtor, non-copyable); value types that shouldn't be copied use `COMMON_DECLARE_NON_COPY`. Both
  are defined in `Common/External/Common.h`.
- `_HAS_EXCEPTIONS 0` / MSVC `/EHs-c-` — exceptions are compiled out engine-wide; use
  `Common::FatalError` / `COMMON_ASSERT` (debug-only, via `DEBUG_CHECKS`) for error handling
  instead of `throw`.
- Formatting is enforced by `.clang-format` (LLVM-based, 4-space indent, 100-col limit, Allman-ish
  custom brace wrapping, left-aligned pointers) — run `clang-format` before committing C++ changes.
