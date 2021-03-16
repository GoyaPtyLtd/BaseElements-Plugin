#!/bin/bash

#!/bin/bash

# Based on

#  Compile-Freetype-For-iOS
#  Original Script https://github.com/jkyin/Compile-Freetype-For-iOS/blob/master/build_freetype.sh
#  Revised by: l'L'l
#
#  New Features Include: auto download latest version, fixed toolchain locations, other various tweeks
#
#  The MIT License (MIT)
#  Copyright (c) 2016 l'L'l

########################################################################
# Configuration
########################################################################

SRCROOT=/Users/mark/Dropbox/Development/Plugin_Build_Folder/BaseElements

########################################################################
# Main
########################################################################

cd podofo-0.9.7
mkdir _build_ios
cd _build_ios


## arm64
echo "--- Building for arm64 ---"

IOS_SYSROOT=$(xcrun --sdk iphoneos --show-sdk-path)
cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=${IOS_SYSROOT[0]} ../. -DCMAKE_INSTALL_PREFIX="./_build_ios" -DWANT_FONTCONFIG:BOOL=TRUE -DCMAKE_BUILD_TYPE=RELEASE -DPODOFO_BUILD_STATIC:BOOL=TRUE -DPODOFO_BUILD_SHARED:BOOL=FALSE -DFREETYPE_INCLUDE_DIR="${SRCROOT}/Headers/freetype2" -DFREETYPE_LIBRARY_RELEASE="${SRCROOT}/Libraries/iOS/libfreetype.a" -DFONTCONFIG_LIBRARIES="${SRCROOT}/Libraries/iOS" -DFONTCONFIG_INCLUDE_DIR="${SRCROOT}/Headers" -DPODOFO_BUILD_LIB_ONLY=TRUE -DCMAKE_C_FLAGS="-arch arm64 -miphoneos-version-min=13.2 -stdlib=libc++" -DCMAKE_CXX_FLAGS="-arch arm64 -miphoneos-version-min=13.2 -stdlib=libc++" -DCMAKE_CXX_STANDARD=11
make install
mv -v src/podofo/libpodofo.a libpodofo_arm64_a


##  x86_64
echo "--- Building for x86_64 ---"

IOS_SIMULATOR_SYSROOT=$(xcrun --sdk iphonesimulator --show-sdk-path)
cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=${IOS_SIMULATOR_SYSROOT[0]} ../ -DCMAKE_INSTALL_PREFIX="./_build_iossimulator" -DWANT_FONTCONFIG:BOOL=TRUE -DCMAKE_BUILD_TYPE=RELEASE -DPODOFO_BUILD_STATIC:BOOL=TRUE -DPODOFO_BUILD_SHARED:BOOL=FALSE -DFREETYPE_INCLUDE_DIR="${SRCROOT}/Headers/freetype2" -DFREETYPE_LIBRARY_RELEASE="${SRCROOT}/Libraries/iOS/libfreetype.a" -DFONTCONFIG_LIBRARIES="${SRCROOT}/Libraries/iOS" -DFONTCONFIG_INCLUDE_DIR="${SRCROOT}/Headers" -DPODOFO_BUILD_LIB_ONLY=TRUE -DCMAKE_C_FLAGS="-arch x86_64 -miphonesimulator-version-min=13.2 -stdlib=libc++" -DCMAKE_CXX_FLAGS="-arch x86_64 -miphonesimulator-version-min=13.2 -stdlib=libc++" -DCMAKE_CXX_STANDARD=11
make install
mv -v src/podofo/libpodofo.a libpodofo_x86_64_a


## lipo
# Universal fat binary
xcrun -sdk iphoneos lipo libpodofo_arm64_a libpodofo_x86_64_a -create -output libpodofo.a

########################################################################
# End
########################################################################

