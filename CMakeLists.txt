# BaseElements-Plugin CMake Build Configuration
#
# This file tells CMake how to build the BaseElements FileMaker plugin.
# It automatically detects your system (Ubuntu version, CPU architecture) and
# configures the build accordingly.
#
# Usage:
#   mkdir build
#   cd build
#   cmake -DCMAKE_BUILD_TYPE=Release ..
#   make -j$(($(nproc) + 1))
#   make install
#   sudo chown -f fmserver:fmsadmin "/opt/FileMaker/FileMaker Server/Database Server/Extensions/BaseElements.fmx"
#
# Build Options:
#   -DCMAKE_BUILD_TYPE=Debug      # Build with debug symbols (default)
#   -DCMAKE_BUILD_TYPE=Release    # Build optimized version (smaller, faster)
#   -DCMAKE_INSTALL_PREFIX=...    # Override default install location
#
# Copyright (C) 2024 Goya Pty Ltd
# Author: Gavin Stewart <gavin@goya.com.au>

cmake_minimum_required(VERSION 3.16.3)

project(BaseElements-Plugin)

# ============================================================================
# CONFIGURATION VARIABLES
# ============================================================================
# These can be changed to customize the build

set(SONAME                          "5.0.0")  # Plugin version identifier
set(INSTALL_OWNER                   "fmserver:fmsadmin")  # File ownership after install
set(DEFAULT_INSTALL_PREFIX          "/opt/FileMaker/FileMaker Server/Database Server/Extensions")

# ============================================================================
# AUTOMATIC SYSTEM DETECTION
# ============================================================================
# Detect what type of computer we're building on (x86_64 or ARM64)

# Step 1: Detect CPU architecture (x86_64 or ARM64)
execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE NATIVE_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NATIVE_ARCH MATCHES "aarch64|arm64")
    # We're building on an ARM64
    set(TARGET_ARCH "aarch64")
    set(TARGET_ARCH_DIR "arm64")  # FileMaker SDK uses "arm64" as directory name
    set(TARGET_LIB_DIR "aarch64-linux-gnu")  # System library directory
    set(ARCH_FLAG "")  # ARM doesn't need -m64 flag
else()
    # We're building on an x86_64
    set(TARGET_ARCH "x86_64")
    set(TARGET_ARCH_DIR "x64")  # FileMaker SDK uses "x64" as directory name
    set(TARGET_LIB_DIR "x86_64-linux-gnu")  # System library directory
    set(ARCH_FLAG "-m64")  # Tell compiler to build 64-bit code
endif()

# Step 2: Detect Ubuntu version (22.04 or 24.04)
# This determines which FileMaker SDK libraries to use
execute_process(
    COMMAND sh -c "lsb_release -rs 2>/dev/null || (cat /etc/os-release | grep VERSION_ID | cut -d'\"' -f2 | cut -d'.' -f1)"
    OUTPUT_VARIABLE UBUNTU_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(UBUNTU_VERSION MATCHES "^22")
    set(UBUNTU_SDK_DIR "U22")  # Use Ubuntu 22 SDK
elseif(UBUNTU_VERSION MATCHES "^24")
    set(UBUNTU_SDK_DIR "U24")  # Use Ubuntu 24 SDK
else()
    # If we can't detect, default to Ubuntu 24
    set(UBUNTU_SDK_DIR "U24")
    message(WARNING "Could not detect Ubuntu version, defaulting to U24")
endif()

# Step 3: Build the platform directory name (e.g., "ubuntu24.04-x86_64")
# This matches the directory structure used by the library build system
set(PLATFORM_DIR "ubuntu${UBUNTU_VERSION}-${TARGET_ARCH}")

# Print what we detected (helpful for debugging)
message(STATUS "Detected native architecture: ${NATIVE_ARCH} -> ${TARGET_ARCH}")
message(STATUS "Detected Ubuntu version: ${UBUNTU_VERSION} -> Using ${UBUNTU_SDK_DIR}")
message(STATUS "Using FileMaker SDK: external/${PLATFORM_DIR}/PlugInSDK/")
message(STATUS "Using internal platform directory: internal/${PLATFORM_DIR}")
message(STATUS "Using external platform directory: external/${PLATFORM_DIR}")

# ============================================================================
# COMPILER SETTINGS
# ============================================================================
# Configure how the code gets compiled

# Use Clang compiler (not GCC) for consistency across platforms
set(CMAKE_C_COMPILER                "clang")
set(CMAKE_CXX_COMPILER              "clang++")

# C compiler flags (for C source files like duktape.c)
set(CMAKE_C_FLAGS                   "-Wno-enum-constexpr-conversion -Wno-error=enum-constexpr-conversion -std=gnu11 -fPIC ${ARCH_FLAG} -Wall -fexceptions -Wno-unknown-pragmas -fvisibility=hidden -D_LIBCPP_STD_VER=17")
set(CMAKE_C_FLAGS_DEBUG             "-g")  # Include debug symbols
set(CMAKE_C_FLAGS_RELEASE           "-O2")  # Optimize for size/speed
set(CMAKE_C_FLAGS_RELWITHDEBINFO    "-O2 -g")  # Optimize but keep debug info

# C++ compiler flags (for C++ source files like .cpp files)
set(CMAKE_CXX_FLAGS                 "-Wno-enum-constexpr-conversion -Wno-error=enum-constexpr-conversion -fPIC ${ARCH_FLAG} -Wall -fexceptions -Wno-unknown-pragmas -fvisibility=hidden -D_LIBCPP_STD_VER=17")
set(CMAKE_CXX_FLAGS_DEBUG           "-g")  # Include debug symbols
set(CMAKE_CXX_FLAGS_RELEASE         "-O2")  # Optimize for size/speed
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")  # Optimize but keep debug info

# Linker flags (how to combine compiled files into the final plugin)
set(CMAKE_MODULE_LINKER_FLAGS       "${ARCH_FLAG} -Wl,-soname=${SONAME} -Wl,--exclude-libs,ALL -Wl,--no-undefined")
# Strip debug symbols in Release builds (makes file smaller, matches older CodeBlocks behavior)
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-s")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "-s")

# Optional: Build Pro version (if -DPRO=1 is passed to cmake)
if(PRO)
    set(CMAKE_C_FLAGS               "${CMAKE_C_FLAGS} -DBEP_PRO_VERSION=${PRO}")
    set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -DBEP_PRO_VERSION=${PRO}")
endif()

# ============================================================================
# INSTALLATION SETTINGS
# ============================================================================

# Set where the plugin should look for libraries when running on FileMaker Server
set(CMAKE_INSTALL_RPATH             "/opt/FileMaker/lib")
# Set default installation directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${DEFAULT_INSTALL_PREFIX}")
endif()
# DOWNLOAD_EXTRACT_TIMESTAMP was added in CMake 3.24; older versions (e.g. Ubuntu 22 / CMake 3.22)
# treat it as part of URL_HASH and error out, so only enable it when supported.
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
    list(APPEND FM_PLUGIN_SDK_DOWNLOAD_ARGS DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
endif()
# ============================================================================
# FILEMAKER PLUGIN SDK
# ============================================================================
# The FileMaker Plugin SDK is provided as part of the external libraries
# It is pre-extracted to external/${PLATFORM_DIR}/PlugInSDK/
# Headers: external/${PLATFORM_DIR}/PlugInSDK/Headers
# Libraries: external/${PLATFORM_DIR}/PlugInSDK/Libraries/Linux/${UBUNTU_SDK_DIR}/${TARGET_ARCH_DIR}

# ============================================================================
# PLUGIN SOURCE FILES
# ============================================================================
# List all the source code files that need to be compiled into the plugin
# These are the .cpp (C++) and .c (C) files that make up the plugin

add_library(
    BaseElements

    MODULE  # This creates a loadable plugin module (not a regular library)

    # External source files (from external library build system)
    external/${PLATFORM_DIR}/src/duktape/duktape.c  # JavaScript engine

    # Internal source files (plugin's own code)
    internal/src/BECppUtilities.cpp
    internal/src/BEDebugInformation.cpp
    internal/src/BEFileMakerPlugin.cpp
    internal/src/BEFileSystem.cpp
    internal/src/BEFileTextReader.cpp
    internal/src/BEJSON.cpp
    internal/src/BEJavaScript.cpp
    internal/src/BEPDF.cpp
    internal/src/BEPlugin.cpp
    internal/src/BEPluginFunctions.cpp
    internal/src/BEPluginUtilities.cpp
    internal/src/BEQuadChar.cpp
    internal/src/BESQLCommand.cpp
    internal/src/BESystemCommand.cpp
    internal/src/BETask.cpp
    internal/src/BETask.h
    internal/src/BETime.cpp
    internal/src/BEXMLReader.cpp
    internal/src/BEXMLReaderInterface.cpp
    internal/src/BEXMLSchema.cpp
    internal/src/BEXMLTextReader.cpp
    internal/src/BEXMLTextWriter.cpp
    internal/src/BEXSLT.cpp
    internal/src/BEZlib.cpp
    # Crypto functions
    internal/src/Crypto/BEBase64.cpp
    internal/src/Crypto/BEBio.cpp
    internal/src/Crypto/BEMessageDigest.cpp
    internal/src/Crypto/BEOpenSSLAES.cpp
    internal/src/Crypto/BEOpenSSLRSA.cpp
    internal/src/Crypto/BEOpenSSLRSA.h
    # Image processing
    internal/src/Images/BEImage.cpp
    internal/src/Images/BEJPEG.cpp
    # Network functions
    internal/src/Net/BECurl.cpp
    internal/src/Net/BECurlOption.cpp
    internal/src/Net/BEMailRecipient.cpp
    internal/src/Net/BESMTP.cpp
    internal/src/Net/BESMTPContainerAttachments.cpp
    internal/src/Net/BESMTPEmailMessage.cpp
    # Platform-specific code
    internal/src/linux/BELinuxFunctions.cpp
)

# ============================================================================
# HEADER FILE SEARCH PATHS
# ============================================================================
# Tell the compiler where to find header files (.h, .hpp) when compiling
# These directories are searched when the code does #include <something.h>

target_include_directories(
    BaseElements PUBLIC

    # Plugin's own header files (headers are alongside source files in internal/src/)
    internal/src
    
    # FileMaker SDK headers (downloaded by ExternalProject)
    external/${PLATFORM_DIR}/PlugInSDK/Headers
    # ImageMagick headers need special path (they're nested deeper)
    external/${PLATFORM_DIR}/include/ImageMagick-7
    # External library headers (from BaseElements-Plugin-Libraries build)
    # Note: These directories are searched recursively - the compiler will look in
    # all subdirectories (e.g., include/boost/, include/Poco/, etc.)
    external/${PLATFORM_DIR}/include
    
    external/${PLATFORM_DIR}/lib  # Some libraries put headers in lib/ subdirectories
    external/${PLATFORM_DIR}/src  # External source files (e.g., duktape headers)
    

)

# ============================================================================
# LIBRARY SEARCH PATHS
# ============================================================================
# Tell the linker where to find pre-built libraries (.a, .so files) to link against
# These are the compiled libraries that the plugin depends on

target_link_directories(
    BaseElements PUBLIC

    # FileMaker SDK libraries (downloaded by ExternalProject)
    external/${PLATFORM_DIR}/PlugInSDK/Libraries/Linux/${UBUNTU_SDK_DIR}/${TARGET_ARCH_DIR}
    
    # External library files (from BaseElements-Plugin-Libraries build)
    external/${PLATFORM_DIR}/lib

)

# ============================================================================
# SYSTEM LIBRARIES
# ============================================================================
# Link against the system's UUID library (for generating unique IDs)

add_library(libuuid SHARED IMPORTED)
set_target_properties(
    libuuid PROPERTIES
        IMPORTED_LOCATION /usr/lib/${TARGET_LIB_DIR}/libuuid.so.1)

# ============================================================================
# LIBRARIES TO LINK
# ============================================================================
# List all the libraries that the plugin needs to link against
# The linker will search for these in the directories specified above

target_link_libraries(
    BaseElements PUBLIC

    # FileMaker SDK
    FMWrapper
    
    # ImageMagick (image processing)
    Magick++-7.Q16HDRI
    MagickWand-7.Q16HDRI
    MagickCore-7.Q16HDRI
    
    # Boost libraries (C++ utilities)
    #boost_atomic  # Not currently used
    boost_date_time
    boost_filesystem
    boost_program_options
    boost_regex
    boost_thread
    
    # Network libraries
    #nghttp2  # Not currently used
    curl      # HTTP client
    PocoNet   # Network utilities
    
    # Security libraries
    ssh2      # SSH client
    ssl       # SSL/TLS
    crypto    # Cryptography
    
    # XML/XSLT processing
    xslt      # XSLT transformations
    exslt     # Extended XSLT
    xml2      # XML parsing
    
    # Image format libraries
    turbojpeg # JPEG processing
    jpeg      # JPEG support
    png16     # PNG support
    z         # Compression
    
    # Poco libraries (C++ framework)
    PocoJSON
    PocoZip
    PocoXML
    #PocoUtil  # Windows-only, not available for Linux
    
    # PDF processing
    podofo
    
    # Font and text processing
    unistring  # Unicode helpers required by PoDoFo
    fontconfig # Font configuration
    freetype   # Font rendering
    iconv      # Character encoding conversion
    charset    # Character set support
    
    # Image format libraries (continued)
    heif       # HEIF image format
    de265      # HEVC video codec
    jq         # JSON query processor
    
    # Poco libraries (continued)
    PocoCrypto
    PocoFoundation
    
    # XML processing (continued)
    expat      # XML parser
    
    # System libraries
    libuuid    # UUID generation
    dl         # Dynamic loading
    pthread    # Threading support
    m          # Math library
)

# Link openjp2 only on macOS (not built on Linux)
if(APPLE)
    target_link_libraries(BaseElements PUBLIC openjp2)  # JPEG 2000 support
endif()

# ============================================================================
# BUILD DEPENDENCIES
# ============================================================================
# No external dependencies needed - all libraries are pre-extracted in external/

# ============================================================================
# OUTPUT FILE CONFIGURATION
# ============================================================================
# Configure how the final plugin file is named and where it looks for libraries

set_target_properties(
    BaseElements PROPERTIES
        PREFIX ""           # Don't add "lib" prefix (we want "BaseElements.fmx" not "libBaseElements.fmx")
        SUFFIX ".fmx"       # Use .fmx extension instead of .so
        BUILD_WITH_INSTALL_RPATH TRUE  # Use install RPATH during build
        INSTALL_RPATH "/opt/FileMaker/lib"  # Where to look for libraries on FileMaker Server
        BUILD_RPATH "/opt/FileMaker/lib")   # Where to look for libraries during build

# ============================================================================
# POST-BUILD ACTIONS
# ============================================================================
# After building, create a timestamped copy and generate a checksum file
# This lets you keep a history of all builds

# Generate timestamp at configure time
string(TIMESTAMP BUILD_TIME "%Y%m%d_%H%M%S")

# Set source and destination file paths
set(SRC_FILE "$<TARGET_FILE:BaseElements>")
set(DST_FILE "$<TARGET_FILE_DIR:BaseElements>/BaseElements-${BUILD_TIME}.fmx")

add_custom_command(TARGET BaseElements POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Creating timestamped copy: BaseElements-${BUILD_TIME}.fmx"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SRC_FILE}" "${DST_FILE}"
    COMMAND ${CMAKE_COMMAND} -E echo "Generating SHA256 checksum..."
    COMMAND ${CMAKE_COMMAND} -E sha256sum "${DST_FILE}" >> "SHA256SUM"
    COMMAND ${CMAKE_COMMAND} -E echo "Build saved with timestamp and checksum appended"
    COMMENT "Creating timestamped copy and appending SHA256 checksum for BaseElements.fmx"
)

# ============================================================================
# INSTALLATION
# ============================================================================
# Configure how to install the plugin to FileMaker Server

# Install the plugin file
install(
    TARGETS BaseElements
    LIBRARY DESTINATION .)

# Set correct file ownership after installation
install(CODE "execute_process(COMMAND chown -f \"${INSTALL_OWNER}\" \"${CMAKE_INSTALL_PREFIX}/BaseElements.fmx\" COMMAND_ECHO STDOUT)")
