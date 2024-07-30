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
#
# Copyright (C) 2024 Goya Pty Ltd
#
# Author: Gavin Stewart <gavin@goya.com.au>

cmake_minimum_required(VERSION 3.22)

project(BaseElements-Plugin)

# Configurable variables
set(SONAME "5.1.0")

# Compiler and linker flags
set(CMAKE_C_COMPILER                "clang")
set(CMAKE_C_FLAGS                   "-std=gnu11 -fPIC -m64 -Wall -fexceptions -Wno-unknown-pragmas -fvisibility=hidden -D_LIBCPP_STD_VER=17")
set(CMAKE_C_FLAGS_DEBUG             "-g")
set(CMAKE_C_FLAGS_RELEASE           "-O2")
set(CMAKE_C_FLAGS_RELWITHDEBINFO    "-O2 -g")

set(CMAKE_CXX_COMPILER              "clang++")
set(CMAKE_CXX_FLAGS                 "-fPIC -m64 -Wall -fexceptions -Wno-unknown-pragmas -fvisibility=hidden -D_LIBCPP_STD_VER=17")
set(CMAKE_CXX_FLAGS_DEBUG           "-g")
set(CMAKE_CXX_FLAGS_RELEASE         "-O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")

set(CMAKE_MODULE_LINKER_FLAGS       "-m64 -Wl,-soname=${SONAME} -Wl,--exclude-libs,ALL -Wl,--no-undefined")

set(CMAKE_INSTALL_PREFIX            "/opt/FileMaker/FileMaker Server/Database Server/Extensions")
set(CMAKE_INSTALL_RPATH             "/opt/FileMaker/lib")

# What we are building
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
    BaseElements
    PROPERTIES

    PREFIX ""           # We don't want the "lib" prefix
    SUFFIX ".fmx"       # We don't want the ".so" suffix
)

target_include_directories(
    BaseElements

    PUBLIC

    Source
    PlugInSDK/Headers
    BaseElements-Plugin-Libraries/Output/Headers
    BaseElements-Plugin-Libraries/Output/Headers/ImageMagick-7
)

target_link_directories(
    BaseElements

    PUBLIC

    PlugInSDK/Libraries/Linux/U22/x64
    BaseElements-Plugin-Libraries/Output/Libraries/linux
)

add_library(libuuid SHARED IMPORTED)
set_target_properties(libuuid PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libuuid.so.1)

target_link_libraries(
    BaseElements

    PUBLIC

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
    openjp2
    PocoUtil
    PocoPDF
    PocoCrypto
    PocoFoundation
    charset
    expat
    libuuid
    dl
    pthread
    m
)

install(TARGETS BaseElements LIBRARY DESTINATION .)
