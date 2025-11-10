# Internal Libraries and Headers

This directory contains libraries and headers that remain in the main repository (tracked by git) rather than being provided by the external library build system.

## Structure

- `src/` - Plugin source files (.cpp, .c) and headers (.h) - **shared across all platforms**
  - Headers are alongside source files (e.g., `src/BEPlugin.cpp` and `src/BEPlugin.h`)
  - CMakeLists.txt uses `internal/src` as an include directory

- `include/` - Shared header files (.h, .hpp) for third-party libraries - **shared across all platforms**
  - Most headers are platform-agnostic and work on all platforms
  - Examples: FMWrapper, Boost, Poco, ImageMagick, zlib, etc.
  - CMakeLists.txt uses `internal/include` as an include directory

Platform-specific folders matching the BaseElements-Plugin-Libraries output structure:

- `ubuntu24.04-x86_64/{include,lib}/` - Ubuntu 24.04 x86_64
- `ubuntu24.04-aarch64/{include,lib}/` - Ubuntu 24.04 ARM64
- `ubuntu22.04-x86_64/{include,lib}/` - Ubuntu 22.04 x86_64
- `ubuntu22.04-aarch64/{include,lib}/` - Ubuntu 22.04 ARM64
- `macos-arm64-x86_64/{include,lib}/` - macOS (universal binary)
- `win64/{include,lib}/` - Windows

Each platform directory contains:
- `include/` - Platform-specific header files (only for headers that differ by platform, e.g., ImageMagick-7 headers that are platform-specific)
- `lib/` - Compiled library files (.so, .a, .lib, .framework) for that platform

## What goes here

Files that should be placed in `internal/`:

1. **Plugin source code** - The plugin's own C++ source files
   - Source: `internal/src/`
   - Headers: `internal/src/` (alongside source files)

2. **Shared third-party headers** - Headers that work on all platforms
   - Headers: `internal/include/`
   - Examples: FMWrapper, Boost, Poco, ImageMagick, zlib, etc.
   - Most headers are platform-agnostic and belong here

3. **Platform-specific headers** - Headers that differ by platform (rare)
   - Headers: `internal/${PLATFORM}/include/`
   - Only use this for headers that truly differ between platforms

4. **Platform-specific libraries** - Compiled libraries for each platform
   - Libraries: `internal/${PLATFORM}/lib/`
   - Examples: `libFMWrapper.so` for Linux, `.framework` files for macOS

## Relationship to external/

- `internal/` - Files tracked in this repository (git)
- `external/` - Files from BaseElements-Plugin-Libraries build system or pre-built releases
