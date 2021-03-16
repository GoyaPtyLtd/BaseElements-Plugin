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
# Main
########################################################################

BUILD_DIR=$HOME/Downloads/libiconv_iOS_Release # RELEASE_DIR
mkdir -p ${BUILD_DIR}
cd $BUILD_DIR
cd "../libiconv-1.16"

set -e

iphoneos="10"
export CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"

ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -pipe -mdynamic-no-pic -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk"
export AR="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar"
export LDFLAGS="-arch ${ARCH} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk -miphoneos-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --disable-shared --enable-static --prefix=${BUILD_DIR}
make clean
make install
cp lib/.libs/libiconv.a "${BUILD_DIR}/libiconv-${ARCH}.a"
cp libcharset/lib/.libs/libcharset.a "${BUILD_DIR}/libcharset-${ARCH}.a"
build_one="$ARCH"


iphoneos="10"

ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -pipe  -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
export LDFLAGS="-arch ${ARCH} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk -miphoneos-version-min=$iphoneos"
./configure --disable-shared --enable-static --host="${ARCH}-apple-darwin" --prefix=${BUILD_DIR}
make clean
make install
cp lib/.libs/libiconv.a "${BUILD_DIR}/libiconv-${ARCH}.a"
cp libcharset/lib/.libs/libcharset.a "${BUILD_DIR}/libcharset-${ARCH}.a"
build_four="$ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

lipo -create "${BUILD_DIR}/libiconv-arm64.a" "${BUILD_DIR}/libiconv-x86_64.a" -output "${BUILD_DIR}/libiconv.a"
lipolog="$(lipo -info ${BUILD_DIR}/libiconv.a)"
lipo -create "${BUILD_DIR}/libcharset-arm64.a" "${BUILD_DIR}/libcharset-x86_64.a" -output "${BUILD_DIR}/libcharset.a"
lipolog="$(lipo -info ${BUILD_DIR}/libcharset.a)"

echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'
echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################
