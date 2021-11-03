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
cd libssh2-1.9.0

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

export CFLAGS="-arch ${ARCH} -I${SRCROOT}/Headers -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk"
export LDFLAGS="-arch ${ARCH} -L${SRCROOT}/Libraries/iOS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk -miphoneos-version-min=$iphoneos"
./configure --disable-shared --host="aarch64-apple-darwin" --with-openssl --with-libz --disable-examples-build -without-libssl-prefix --disable-debug
make clean
make

cp src/.libs/libssh2.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libssh2.a"

build_one="Device: $ARCH"

########################################################################
# Simulator
########################################################################

ARCH="arm64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -I${SRCROOT}/Headers -miphonesimulator-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
export LDFLAGS="-arch ${ARCH} -L${SRCROOT}/Libraries/iOS/iPhoneOSSimulator -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk -miphonesimulator-version-min=$iphoneos"
./configure --disable-shared --host="aarch64-apple-darwin" --with-openssl --with-libz --disable-examples-build -without-libssl-prefix --disable-debug
make clean
make

cp src/.libs/libssh2.a "libssh2-${ARCH}.a"

build_three="Sim: $ARCH"


ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -I${SRCROOT}/Headers -miphonesimulator-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
export LDFLAGS="-arch ${ARCH} -L${SRCROOT}/Libraries/iOS/iPhoneOSSimulator -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk -miphonesimulator-version-min=$iphoneos"
./configure --disable-shared --host="aarch64-apple-darwin" --with-openssl --with-libz --disable-examples-build -without-libssl-prefix --disable-debug
make clean
make

cp src/.libs/libssh2.a "libssh2-${ARCH}.a"

build_four="Sim: $ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

# generate fat binary

lipo -create libssh2-arm64.a libssh2-x86_64.a -output libssh2.a
lipolog="$(lipo -info libssh2.a)"
cp libssh2.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libssh2.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################
