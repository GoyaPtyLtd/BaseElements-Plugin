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

BUILD_DIR=$HOME/Downloads/openssl_iOS_Release # RELEASE_DIR
mkdir -p ${BUILD_DIR}
cd $BUILD_DIR
cd "../openssl-OpenSSL_1_1_1-stable"

set -e

SRCROOT="/Users/mark/Dropbox/Development/Plugin_Build_Folder/BaseElements"

iphoneos="10"

ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk"
export AR="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar"
export LDFLAGS="-arch ${ARCH} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk -miphoneos-version-min=$iphoneos"
./configure iphoneos-cross no-engine no-hw no-shared
make clean
make
cp libssl.a "${BUILD_DIR}/libssl-${ARCH}.a"
cp libcrypto.a "${BUILD_DIR}/libcrypto-${ARCH}.a"
build_one="$ARCH"


ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
export LDFLAGS="-arch ${ARCH} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk -miphoneos-version-min=$iphoneos"
./configure darwin64-x86_64-cc no-engine no-hw no-shared
make clean
make
cp libssl.a "${BUILD_DIR}/libssl-${ARCH}.a"
cp libcrypto.a "${BUILD_DIR}/libcrypto-${ARCH}.a"
build_four="$ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

lipo -create "${BUILD_DIR}/libssl-arm64.a" "${BUILD_DIR}/libssl-x86_64.a" -output "${BUILD_DIR}/libssl.a"
lipolog="$(lipo -info ${BUILD_DIR}/libssl.a)"
lipo -create "${BUILD_DIR}/libcrypto-arm64.a" "${BUILD_DIR}/libcrypto-x86_64.a" -output "${BUILD_DIR}/libcrypto.a"
lipolog="$(lipo -info ${BUILD_DIR}/libcrypto.a)"


echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'
echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################

