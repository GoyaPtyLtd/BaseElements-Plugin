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


SRCROOT="/Users/mark/Dropbox/Development/Plugin_Build_Folder/BaseElements"
iphoneos="13.2"
cd jansson-master

########################################################################
# Main
########################################################################

set -e

# export CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
# export AR="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar"

########################################################################
# Device
########################################################################

IOS_SDK=$(xcrun --sdk iphoneos --show-sdk-path)

ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphoneos-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphoneos-version-min=$iphoneos"
./configure --disable-shared --host="aarch64-apple-darwin"
make clean
make

cp src/.libs/libjansson.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libjansson.a"

build_one="Device: $ARCH"

########################################################################
# Simulator
########################################################################

IOS_SDK=$(xcrun --sdk iphonesimulator --show-sdk-path)

ARCH="arm64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
./configure --disable-shared --host="aarch64-apple-darwin"
make clean
make

cp src/.libs/libjansson.a "libjansson-${ARCH}.a"

build_three="Sim: $ARCH"


ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
./configure --disable-shared --host="aarch64-apple-darwin"
make clean
make

cp src/.libs/libjansson.a "libjansson-${ARCH}.a"

build_four="Sim: $ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

lipo -create libjansson-arm64.a libjansson-x86_64.a -output libjansson.a
lipolog="$(lipo -info libjansson.a)"
cp libjansson.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libjansson.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################

