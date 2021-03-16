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

BUILD_DIR=$HOME/Downloads/liboauth_iOS_Release # RELEASE_DIR
mkdir -p ${BUILD_DIR}
cd $BUILD_DIR
cd "../liboauth-1.0.3"

set -e

SRCROOT="/Users/mark/Dropbox/Development/Plugin_Build_Folder/BaseElements"

iphoneos="10.0"
export CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"

ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -I${SRCROOT}/Headers -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk"
export AR="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar"
export LDFLAGS="-arch ${ARCH} -L${SRCROOT}/Libraries/iOS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk -miphoneos-version-min=$iphoneos"
./configure --disable-shared --host="aarch64-apple-darwin" --with-openssl --with-libz --disable-examples-build -without-libssl-prefix --disable-debug
make clean
make
cp src/.libs/liboauth.a "${BUILD_DIR}/liboauth-${ARCH}.a"
build_one="$ARCH"


ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -I${SRCROOT}/Headers -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
export LDFLAGS="-arch ${ARCH} -L${SRCROOT}/Libraries/iOS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk -miphoneos-version-min=$iphoneos"
./configure --disable-shared --host="${ARCH}-apple-darwin" --with-openssl --with-libz --disable-examples-build -without-libssl-prefix --disable-debug
make clean
make
cp src/.libs/liboauth.a "${BUILD_DIR}/liboauth-${ARCH}.a"
build_four="$ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

lipo -create "${BUILD_DIR}/liboauth-arm64.a" "${BUILD_DIR}/liboauth-x86_64.a" -output "${BUILD_DIR}/liboauth.a"
lipolog="$(lipo -info ${BUILD_DIR}/liboauth.a)"

echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'
echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################
