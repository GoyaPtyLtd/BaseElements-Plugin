# Internal Libraries and Headers

This directory contains libraries and headers that remain in the main repository (tracked by git) rather than being provided by the external library build system.

## Structure

Platform-specific folders matching the BaseElements-Plugin-Libraries output structure:

- `src/` - common source files for all platforms
- `ubuntu24.04-x86_64/{include,lib}/` - Ubuntu 24.04 x86_64
- `ubuntu24.04-aarch64/{include,lib}/` - Ubuntu 24.04 ARM64
- `ubuntu22.04-x86_64/{include,lib}/` - Ubuntu 22.04 x86_64
- `ubuntu22.04-aarch64/{include,lib}/` - Ubuntu 22.04 ARM64
- `macos-arm64-x86_64/{include,lib}/` - macOS (universal binary)
- `win64/{include,lib}/` - Windows

Each platform directory contains:
- `include/` - header files (.h, .hpp) for that platform
- `lib/` - compiled library files (.so, .a, .lib, .framework) for that platform

## What goes here

Files that should be placed in `internal/`:

1. **FMWrapper** - FileMaker SDK headers and libraries
   - Headers: `Headers/FMWrapper/` → `internal/${PLATFORM}/include/FMWrapper/`
   - Libraries: `Libraries/linux/libFMWrapper.so` → `internal/${PLATFORM}/lib/libFMWrapper.so`

2. **Windows-specific files** (if needed for cross-platform builds)
   - `Headers/ImageMagick-7-win64/` → `internal/${PLATFORM}/include/ImageMagick-7-win64/`

3. **Any other files that cannot be provided by the external build system**

## Relationship to external/

- `internal/` - Files tracked in this repository (git)
- `external/` - Files from BaseElements-Plugin-Libraries build system or pre-built releases

## Migration Notes

During migration from `Headers/` and `Libraries/linux/`:
- Most third-party libraries → `external/` (from BaseElements-Plugin-Libraries)
- FMWrapper and FileMaker SDK files → `internal/`
