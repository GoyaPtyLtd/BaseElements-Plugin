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

cmake_minimum_required(VERSION 3.22)

project(BaseElements-Plugin)

# Configurable variables
set(SONAME                          "5.1.0")
set(INSTALL_OWNER                   "fmserver:fmsadmin")
set(DEFAULT_INSTALL_PREFIX          "/opt/FileMaker/FileMaker Server/Database Server/Extensions")

# Compiler and linker flags
set(CMAKE_C_COMPILER                "clang")
set(CMAKE_CXX_FLAGS                 "-std=c++20")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_FLAGS_DEBUG             "-g")
set(CMAKE_C_FLAGS_RELEASE           "-O2")
set(CMAKE_C_FLAGS_RELWITHDEBINFO    "-O2 -g")

set(CMAKE_CXX_COMPILER              "clang++")
set(CMAKE_CXX_FLAGS                 "-fPIC -m64 -Wall -fexceptions -Wno-unknown-pragmas -fvisibility=hidden -D_LIBCPP_STD_VER=20")
set(CMAKE_CXX_FLAGS_DEBUG           "-g")
set(CMAKE_CXX_FLAGS_RELEASE         "-O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")

set(CMAKE_MODULE_LINKER_FLAGS       "-m64 -Wl,-soname=${SONAME} -Wl,--exclude-libs,ALL -Wl,--no-undefined")

# Install configuration
set(CMAKE_INSTALL_RPATH             "/opt/FileMaker/lib")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${DEFAULT_INSTALL_PREFIX}")
endif()

# Platform detection for external/* layout and Linux arch-specific paths
execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE NATIVE_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NATIVE_ARCH MATCHES "aarch64|arm64")
    set(TARGET_ARCH                    "aarch64")
    set(TARGET_ARCH_DIR                "arm64")
    set(TARGET_LIB_DIR                 "aarch64-linux-gnu")
    set(ARCH_FLAG                      "")
else()
    set(TARGET_ARCH                    "x86_64")
    set(TARGET_ARCH_DIR                "x64")
    set(TARGET_LIB_DIR                 "x86_64-linux-gnu")
    set(ARCH_FLAG                      "-m64")
endif()

execute_process(
    COMMAND sh -c "lsb_release -rs 2>/dev/null || (grep VERSION_ID /etc/os-release | tr -d '\"' | cut -d= -f2)"
    OUTPUT_VARIABLE UBUNTU_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(UBUNTU_VERSION MATCHES "^22")
    set(UBUNTU_SDK_DIR                 "U22")
    set(LEGACY_UBUNTU_LIB_DIR          "ubuntu22")
elseif(UBUNTU_VERSION MATCHES "^24")
    set(UBUNTU_SDK_DIR                 "U24")
    set(LEGACY_UBUNTU_LIB_DIR          "ubuntu24")
else()
    set(UBUNTU_SDK_DIR                 "U24")
    set(LEGACY_UBUNTU_LIB_DIR          "ubuntu24")
endif()

set(PLATFORM_DIR                       "ubuntu${UBUNTU_VERSION}-${TARGET_ARCH}")
set(EXTERNAL_PLATFORM_DIR              "${CMAKE_SOURCE_DIR}/external/${PLATFORM_DIR}")
set(DUKTAPE_SOURCE_FILE                "${EXTERNAL_PLATFORM_DIR}/src/duktape/duktape.c")

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
    ${DUKTAPE_SOURCE_FILE}
    Source/linux/BELinuxFunctions.cpp
)

set_target_properties(
    BaseElements PROPERTIES
        PREFIX ""           # We don't want the "lib" prefix
        SUFFIX ".fmx"       # We don't want the ".so" suffix
)

target_include_directories(
    BaseElements PUBLIC
    Source
    "${EXTERNAL_PLATFORM_DIR}/PlugInSDK/Headers"
    "${EXTERNAL_PLATFORM_DIR}/include/ImageMagick-7"
    "${EXTERNAL_PLATFORM_DIR}/include"
    "${EXTERNAL_PLATFORM_DIR}/lib"
    "${EXTERNAL_PLATFORM_DIR}/src"
)

target_link_directories(
    BaseElements PUBLIC
    "${EXTERNAL_PLATFORM_DIR}/PlugInSDK/Libraries/Linux/${UBUNTU_SDK_DIR}/${TARGET_ARCH_DIR}"
    "${EXTERNAL_PLATFORM_DIR}/lib"
)

add_library(libuuid SHARED IMPORTED)
set_target_properties(
    libuuid PROPERTIES
        IMPORTED_LOCATION /usr/lib/${TARGET_LIB_DIR}/libuuid.so.1)

target_link_libraries(
    BaseElements PUBLIC

    FMWrapper
    Magick++-7.Q16HDRI
    MagickWand-7.Q16HDRI
    MagickCore-7.Q16HDRI
    boost_atomic
    boost_date_time
    boost_filesystem
    boost_program_options
    boost_regex
    boost_thread
    nghttp2
    curl
    PocoNet
    ssh2
    xslt
    exslt
    xml2
    turbojpeg
    PocoJSON
    PocoZip
    PocoXML
    podofo
    podofo_private
    podofo_3rdparty
    unistring
    fontconfig
    freetype
    iconv
    heif
    de265
    jpeg
    png16
    z
    jq
    PocoUtil
    PocoCrypto
    PocoFoundation
    ssl
    crypto
    charset
    expat
    libuuid
    dl
    pthread
    m
)

# Install target
install(
    TARGETS BaseElements
    LIBRARY DESTINATION .)

# Try to set file ownsership on installed plugin file
install(CODE "execute_process(COMMAND chown -f \"${INSTALL_OWNER}\" \"${CMAKE_INSTALL_PREFIX}/BaseElements.fmx\" COMMAND_ECHO STDOUT)")
