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

SRCROOT="/Users/mark/Dropbox/Development/Plugin_Build_Folder/BaseElements"
iphoneos="13.2"
cd fontconfig-2.13.1

# Note: remove occurances of 'test' from subdirs in Makefile.in

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
export LDFLAGS="-arch ${ARCH} -lbz2 -lz -isysroot ${IOS_SDK} -miphoneos-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --disable-docs --enable-static FREETYPE_CFLAGS="-I${SRCROOT}/Headers/freetype2" FREETYPE_LIBS="-L${SRCROOT}/Libraries/iOS/iPhoneOS -lfreetype" 
make clean
make

cp src/.libs/libfontconfig.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libfontconfig.a"

build_one="Device: $ARCH"
# make distclean


########################################################################
# Simulator
########################################################################

IOS_SDK=$(xcrun --sdk iphonesimulator --show-sdk-path)

ARCH="arm64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -lbz2 -lz -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --disable-docs --enable-static FREETYPE_CFLAGS="-I${SRCROOT}/Headers/freetype2" FREETYPE_LIBS="-L${SRCROOT}/Libraries/iOS/iPhoneOSSimulator -lfreetype" 
make clean
make

cp src/.libs/libfontconfig.a "libfontconfig-${ARCH}.a"

build_three="Sim: $ARCH"


ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -lbz2 -lz -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --disable-docs --enable-static FREETYPE_CFLAGS="-I${SRCROOT}/Headers/freetype2" FREETYPE_LIBS="-L${SRCROOT}/Libraries/iOS/iPhoneOSSimulator -lfreetype" 
make clean
make

cp src/.libs/libfontconfig.a "libfontconfig-${ARCH}.a"

build_four="Sim: $ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

lipo -create libfontconfig-arm64.a libfontconfig-x86_64.a -output libfontconfig.a
lipolog="$(lipo -info libfontconfig.a)"
cp libfontconfig.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libfontconfig.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################

