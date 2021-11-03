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
iphoneos="13.2"
cd podofo-0.9.7


########################################################################
# Main
########################################################################

set -e

# export CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
# export AR="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar"


########################################################################
# Device
########################################################################

ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

IOS_SYSROOT=$(xcrun --sdk iphoneos --show-sdk-path)
cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=${IOS_SYSROOT} -DCMAKE_INSTALL_PREFIX="./_build_ios" -DWANT_FONTCONFIG:BOOL=TRUE -DCMAKE_BUILD_TYPE=RELEASE -DPODOFO_BUILD_STATIC:BOOL=TRUE -DPODOFO_BUILD_SHARED:BOOL=FALSE -DFREETYPE_INCLUDE_DIR="${SRCROOT}/Headers/freetype2" -DFREETYPE_LIBRARY_RELEASE="${SRCROOT}/Libraries/iOS/libfreetype.a" -DFONTCONFIG_LIBRARIES="${SRCROOT}/Libraries/iOS/iPhone" -DFONTCONFIG_INCLUDE_DIR="${SRCROOT}/Headers" -DPODOFO_BUILD_LIB_ONLY=TRUE -DCMAKE_C_FLAGS="-arch ${ARCH} -miphoneos-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_FLAGS="-arch ${ARCH} -miphoneos-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_STANDARD=11
make install

cp _build_ios/lib/libpodofo.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libpodofo.a"

build_one="Device: $ARCH"


########################################################################
# Simulator
########################################################################

ARCH="arm64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

IOS_SIMULATOR_SYSROOT=$(xcrun --sdk iphonesimulator --show-sdk-path)
cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=${IOS_SIMULATOR_SYSROOT} -DCMAKE_INSTALL_PREFIX="./_build_iossimulator_${ARCH}" -DWANT_FONTCONFIG:BOOL=TRUE -DCMAKE_BUILD_TYPE=RELEASE -DPODOFO_BUILD_STATIC:BOOL=TRUE -DPODOFO_BUILD_SHARED:BOOL=FALSE -DFREETYPE_INCLUDE_DIR="${SRCROOT}/Headers/freetype2" -DFREETYPE_LIBRARY_RELEASE="${SRCROOT}/Libraries/iOS/libfreetype.a" -DFONTCONFIG_LIBRARIES="${SRCROOT}/Libraries/iOS/iPhoneOSSimulator" -DFONTCONFIG_INCLUDE_DIR="${SRCROOT}/Headers" -DPODOFO_BUILD_LIB_ONLY=TRUE -DCMAKE_C_FLAGS="-arch ${ARCH} -miphonesimulator-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_FLAGS="-arch ${ARCH} -miphonesimulator-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_STANDARD=11
make install

cp _build_iossimulator_arm64/lib/libpodofo.a "libpodofo_${ARCH}.a"

build_three="Sim: $ARCH"


ARCH="x86_64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

IOS_SIMULATOR_SYSROOT=$(xcrun --sdk iphonesimulator --show-sdk-path)
cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=${IOS_SIMULATOR_SYSROOT} -DCMAKE_INSTALL_PREFIX="./_build_iossimulator_${ARCH}" -DWANT_FONTCONFIG:BOOL=TRUE -DCMAKE_BUILD_TYPE=RELEASE -DPODOFO_BUILD_STATIC:BOOL=TRUE -DPODOFO_BUILD_SHARED:BOOL=FALSE -DFREETYPE_INCLUDE_DIR="${SRCROOT}/Headers/freetype2" -DFREETYPE_LIBRARY_RELEASE="${SRCROOT}/Libraries/iOS/libfreetype.a" -DFONTCONFIG_LIBRARIES="${SRCROOT}/Libraries/iOS/iPhoneOSSimulator" -DFONTCONFIG_INCLUDE_DIR="${SRCROOT}/Headers" -DPODOFO_BUILD_LIB_ONLY=TRUE -DCMAKE_C_FLAGS="-arch ${ARCH} -miphonesimulator-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_FLAGS="-arch ${ARCH} -miphonesimulator-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_STANDARD=11
make install

cp _build_iossimulator_x86_64/lib/libpodofo.a "libpodofo_${ARCH}.a"


build_four="Sim: $ARCH"

# argh !!! we already have a fat binary ???

cp _build_iossimulator_x86_64/lib/libpodofo.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libpodofo.a"
exit

# end argh !!!

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

## lipo
# Universal fat binary
lipo -create libpodofo_arm64.a libpodofo_x86_64.a -output libpodofo.a
lipolog="$(lipo -info libpodofo.a)"
cp libpodofo.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libpodofo.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'


########################################################################
# End
########################################################################

