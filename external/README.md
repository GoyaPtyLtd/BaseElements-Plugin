# External prebuilt libraries

This directory holds platform-specific headers and static/shared libraries produced by the [BaseElements-Plugin-Libraries](https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries) build (or matching release tarballs).

- **Local builds:** extract an `external-<platform>.tar.gz` here so you have `external/ubuntu22.04-x86_64/` (or your platform) with `include/`, `lib/`, `PlugInSDK/`, `src/`, etc.
- **Git:** everything under `external/` except this file is listed in `.gitignore` so large binaries are not committed.

See the project `CMakeLists.txt` and `.github/workflows/main.yml` for how `external/${PLATFORM_DIR}` is used on Linux.
