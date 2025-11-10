# External Libraries and Headers

This directory contains third-party libraries and headers provided by the BaseElements-Plugin-Libraries build system. These files are typically managed via git-lfs due to their size.

## Structure

Platform-specific folders matching the BaseElements-Plugin-Libraries output structure:

- `ubuntu24.04-x86_64/{include,lib,src}/` - Ubuntu 24.04 x86_64
- `ubuntu24.04-aarch64/{include,lib,src}/` - Ubuntu 24.04 ARM64
- `ubuntu22.04-x86_64/{include,lib,src}/` - Ubuntu 22.04 x86_64
- `ubuntu22.04-aarch64/{include,lib,src}/` - Ubuntu 22.04 ARM64
- `macos-arm64-x86_64/{include,lib,src}/` - macOS (universal binary)
- `win64/{include,lib,src}/` - Windows

Each platform directory contains:
- `include/` - header files (.h, .hpp) for third-party libraries
- `lib/` - compiled library files (.so, .a, .lib, .framework) for third-party libraries
- `src/` - source files for libraries that are compiled as part of the plugin build (e.g., duktape)

Additionally:
- `PlugInSDK/` - FileMaker Plugin SDK (automatically downloaded by CMake if not present)

## What goes here

Files that should be placed in `external/`:

1. **Third-party libraries** - Pre-built libraries from BaseElements-Plugin-Libraries
   - Examples: Boost, Poco, ImageMagick, zlib, etc.
   - Headers: `external/${PLATFORM}/include/`
   - Libraries: `external/${PLATFORM}/lib/`

2. **Source-only libraries** - Libraries compiled as part of the plugin build
   - Example: duktape (JavaScript engine)
   - Source: `external/${PLATFORM}/src/`

3. **FileMaker Plugin SDK** - Automatically downloaded by CMake
   - Location: `external/PlugInSDK/`
   - Contains headers and libraries for all platforms
   - Downloaded from Claris website if not present

## Relationship to internal/

- `external/` - Files from BaseElements-Plugin-Libraries build system or pre-built releases
- `internal/` - FileMaker SDK files and other files tracked directly in git

## Building and Updating

To update libraries in this directory, you have two options:

**Option 1: Build yourself**
1. Build libraries using the [BaseElements-Plugin-Libraries](https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries) repository
2. Use the `3copy.sh` script to move the built libraries into place

**Option 2: Use pre-built releases**
1. Download and extract from the appropriate release of pre-built libraries in the [BaseElements-Plugin-Libraries](https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries) repository
2. Copy the extracted libraries to the appropriate platform directories in `external/`


The FileMaker Plugin SDK is automatically downloaded by CMake during the build process if it's not already present in `external/PlugInSDK/`.

