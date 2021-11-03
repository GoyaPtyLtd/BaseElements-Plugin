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
cd "libiconv-1.16"

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
./configure --host="aarch64-apple-darwin" --disable-shared --enable-static
make clean
make

cp lib/.libs/libiconv.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libiconv.a"
cp libcharset/lib/.libs/libcharset.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libcharset.a"

build_one="$ARCH"


########################################################################
# Simulator
########################################################################

IOS_SDK=$(xcrun --sdk iphonesimulator --show-sdk-path)


ARCH="arm64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --disable-shared --enable-static
make clean
make

cp lib/.libs/libiconv.a "libiconv-${ARCH}.a"
cp libcharset/lib/.libs/libcharset.a "libcharset-${ARCH}.a"

build_three="$ARCH"


ARCH="x86_64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --disable-shared --enable-static
make clean
make

cp lib/.libs/libiconv.a "libiconv-${ARCH}.a"
cp libcharset/lib/.libs/libcharset.a "libcharset-${ARCH}.a"

build_four="SIM: $ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

# generate fat binary

lipo -create libiconv-arm64.a libiconv-x86_64.a -output libiconv.a
lipolog="$(lipo -info libiconv.a)"
cp libiconv.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libiconv.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

lipo -create libcharset-arm64.a libcharset-x86_64.a -output libcharset.a
lipolog="$(lipo -info libcharset.a)"
cp libcharset.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libcharset.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################
