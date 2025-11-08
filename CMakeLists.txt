# BaseElements-Plugin CMake file
#
# Usage:
#   mkdir build
#   cd build
#   cmake -DCMAKE_BUILD_TYPE=Release ..
#   make -j$(($(nproc) + 1))
#   make install
#   sudo chown -f fmserver:fmsadmin "/opt/FileMaker/FileMaker Server/Database Server/Extensions/BaseElements.fmx"
#
# Other options:
#   -DCMAKE_BUILD_TYPE=Debug    # This is the default build target
#   -DCMAKE_INSTALL_PREFIX=...  # Override DEFAULT_INSTALL_PREFIX
#
# Copyright (C) 2024 Goya Pty Ltd
#
# Author: Gavin Stewart <gavin@goya.com.au>

cmake_minimum_required(VERSION 3.16.3)

project(BaseElements-Plugin)

# Configurable variables
set(SONAME                          "5.0.0")
set(INSTALL_OWNER                   "fmserver:fmsadmin")
set(DEFAULT_INSTALL_PREFIX          "/opt/FileMaker/FileMaker Server/Database Server/Extensions")

# Detect native CPU architecture
execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE NATIVE_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NATIVE_ARCH MATCHES "aarch64|arm64")
    set(TARGET_ARCH "aarch64")
    set(TARGET_ARCH_DIR "arm64")  # FileMaker SDK uses "arm64" directory name
    set(TARGET_LIB_DIR "aarch64-linux-gnu")
    set(ARCH_FLAG "")  # No -m64 for ARM
else()
    set(TARGET_ARCH "x86_64")
    set(TARGET_ARCH_DIR "x64")
    set(TARGET_LIB_DIR "x86_64-linux-gnu")
    set(ARCH_FLAG "-m64")
endif()

# Detect Ubuntu version for FileMaker SDK path (U22 or U24)
execute_process(
    COMMAND sh -c "lsb_release -rs 2>/dev/null || (cat /etc/os-release | grep VERSION_ID | cut -d'\"' -f2 | cut -d'.' -f1)"
    OUTPUT_VARIABLE UBUNTU_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(UBUNTU_VERSION MATCHES "^22")
    set(UBUNTU_SDK_DIR "U22")
elseif(UBUNTU_VERSION MATCHES "^24")
    set(UBUNTU_SDK_DIR "U24")
else()
    # Default to U24 if detection fails
    set(UBUNTU_SDK_DIR "U24")
    message(WARNING "Could not detect Ubuntu version, defaulting to U24")
endif()

message(STATUS "Detected native architecture: ${NATIVE_ARCH} -> ${TARGET_ARCH}")
message(STATUS "Detected Ubuntu version: ${UBUNTU_VERSION} -> Using ${UBUNTU_SDK_DIR}")
message(STATUS "Using FileMaker SDK path: PlugInSDK/Libraries/Linux/${UBUNTU_SDK_DIR}/${TARGET_ARCH_DIR}")

# Compiler and linker flags
set(CMAKE_C_COMPILER                "clang")
set(CMAKE_C_FLAGS                   "-Wno-enum-constexpr-conversion -Wno-error=enum-constexpr-conversion -std=gnu11 -fPIC ${ARCH_FLAG} -Wall -fexceptions -Wno-unknown-pragmas -fvisibility=hidden -D_LIBCPP_STD_VER=17")
set(CMAKE_C_FLAGS_DEBUG             "-g")
set(CMAKE_C_FLAGS_RELEASE           "-O2")
set(CMAKE_C_FLAGS_RELWITHDEBINFO    "-O2 -g")

set(CMAKE_CXX_COMPILER              "clang++")
set(CMAKE_CXX_FLAGS                 "-Wno-enum-constexpr-conversion -Wno-error=enum-constexpr-conversion -fPIC ${ARCH_FLAG} -Wall -fexceptions -Wno-unknown-pragmas -fvisibility=hidden -D_LIBCPP_STD_VER=17")
set(CMAKE_CXX_FLAGS_DEBUG           "-g")
set(CMAKE_CXX_FLAGS_RELEASE         "-O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")

set(CMAKE_MODULE_LINKER_FLAGS       "${ARCH_FLAG} -Wl,-soname=${SONAME} -Wl,--exclude-libs,ALL -Wl,--no-undefined")
# Strip symbols in Release build (matches CodeBlocks Release build which uses -s)
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-s")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "-s")

# Extend compiler flags if PRO
if(PRO)
    set(CMAKE_C_FLAGS               "${CMAKE_C_FLAGS} -DBEP_PRO_VERSION=${PRO}")
    set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -DBEP_PRO_VERSION=${PRO}")
endif()

# Install configuration
set(CMAKE_INSTALL_RPATH             "/opt/FileMaker/lib")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${DEFAULT_INSTALL_PREFIX}")
endif()

# FileMaker Plugin SDK target
include(ExternalProject)
ExternalProject_Add(
    fm_plugin_sdk
    PREFIX "${CMAKE_BINARY_DIR}/fm_plugin_sdk"  # Download here
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/PlugInSDK"  # Unpack contents here
    # See: https://www.claris.com/resources/downloads/
    URL https://downloads.claris.com/DEVREL/sdk/fm_plugin_sdk_22.0.1.68.zip
    URL_HASH SHA256=ae3ca6b37a96c063e51993d1c7934b3000d9ad27e58996cb99f64b9776df07cd
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

# BaseElements plugin library/module target
add_library(
    BaseElements

    MODULE

    Source/BECppUtilities.cpp
    Source/BEDebugInformation.cpp
    Source/BEFileMakerPlugin.cpp
    Source/BEFileSystem.cpp
    Source/BEFileTextReader.cpp
    Source/BEJSON.cpp
    Source/BEJavaScript.cpp
    Source/BEPDF.cpp
    Source/BEPlugin.cpp
    Source/BEPluginFunctions.cpp
    Source/BEPluginUtilities.cpp
    Source/BEQuadChar.cpp
    Source/BESQLCommand.cpp
    Source/BESystemCommand.cpp
    Source/BETask.cpp
    Source/BETask.h
    Source/BETime.cpp
    Source/BEXMLReader.cpp
    Source/BEXMLReaderInterface.cpp
    Source/BEXMLSchema.cpp
    Source/BEXMLTextReader.cpp
    Source/BEXMLTextWriter.cpp
    Source/BEXSLT.cpp
    Source/BEZlib.cpp
    Source/Crypto/BEBase64.cpp
    Source/Crypto/BEBio.cpp
    Source/Crypto/BEMessageDigest.cpp
    Source/Crypto/BEOpenSSLAES.cpp
    Source/Crypto/BEOpenSSLRSA.cpp
    Source/Crypto/BEOpenSSLRSA.h
    Source/Images/BEImage.cpp
    Source/Images/BEJPEG.cpp
    Source/Net/BECurl.cpp
    Source/Net/BECurlOption.cpp
    Source/Net/BEMailRecipient.cpp
    Source/Net/BESMTP.cpp
    Source/Net/BESMTPContainerAttachments.cpp
    Source/Net/BESMTPEmailMessage.cpp
    Source/duktape/duktape.c
    Source/linux/BELinuxFunctions.cpp
)

set_target_properties(
    BaseElements PROPERTIES
        PREFIX ""           # We don't want the "lib" prefix
        SUFFIX ".fmx"       # We don't want the ".so" suffix
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "/opt/FileMaker/lib"
        BUILD_RPATH "/opt/FileMaker/lib")

target_include_directories(
    BaseElements PUBLIC

    Source
    PlugInSDK/Headers
    Libraries/linux/
    Headers/
    Headers/ImageMagick-7
)

target_link_directories(
    BaseElements PUBLIC

    PlugInSDK/Libraries/Linux/${UBUNTU_SDK_DIR}/${TARGET_ARCH_DIR}
    Libraries/linux
)

add_library(libuuid SHARED IMPORTED)
set_target_properties(
    libuuid PROPERTIES
        IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libuuid.so.1)

target_link_libraries(
    BaseElements PUBLIC

    FMWrapper
    Magick++-7.Q16HDRI
    MagickWand-7.Q16HDRI
    MagickCore-7.Q16HDRI
    #boost_atomic
    boost_date_time
    boost_filesystem
    boost_program_options
    boost_regex
    boost_thread
   #nghttp2
    curl
    PocoNet
    ssh2
    ssl
    crypto
    xslt
    exslt
    xml2
    turbojpeg
    PocoJSON
    PocoZip
    PocoXML
    podofo
   #unistring
    fontconfig
    freetype
    iconv
    heif
    libde265
    jpeg
    png16
    z
    jq
    openjp2
    #PocoUtil  # Not available for Linux, only used in Windows code
    PocoCrypto
    PocoFoundation
    charset
    expat
    libuuid
    dl
    pthread
    m
)

# Make BaseElements target dependent upon fm_plugin_sdk target
add_dependencies(BaseElements fm_plugin_sdk)

# Generate SHA256 checksum after build
add_custom_command(TARGET BaseElements POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E sha256sum $<TARGET_FILE:BaseElements> > $<TARGET_FILE:BaseElements>.sha256
    COMMENT "Generating SHA256 checksum for BaseElements.fmx"
)

# Install target
install(
    TARGETS BaseElements
    LIBRARY DESTINATION .)

# Try to set file ownsership on installed plugin file
install(CODE "execute_process(COMMAND chown -f \"${INSTALL_OWNER}\" \"${CMAKE_INSTALL_PREFIX}/BaseElements.fmx\" COMMAND_ECHO STDOUT)")