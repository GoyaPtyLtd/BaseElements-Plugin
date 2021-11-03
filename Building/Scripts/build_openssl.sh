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
cd "../openssl-OpenSSL_1_1_1-stable"

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

export CFLAGS="-arch ${ARCH} -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk"
export LDFLAGS="-arch ${ARCH} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk -miphoneos-version-min=$iphoneos"
./configure iphoneos-cross no-engine no-hw no-shared
make clean
make

cp libssl.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libssl.a"
cp libcrypto.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libcrypto.a"

build_one="Device: $ARCH"


########################################################################
# Simulator
########################################################################

ARCH="arm64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -miphonesimulator-version-min=${iphoneos} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
export LDFLAGS="-arch ${ARCH} -miphonesimulator-version-min=${iphoneos} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
./configure darwin64-arm64-cc no-engine no-hw no-shared
make clean
make
cp libssl.a "libssl-${ARCH}.a"
cp libcrypto.a "libcrypto-${ARCH}.a"
build_three="SIM: $ARCH"

ARCH="x86_64"
echo "---> Building Simulator ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -miphonesimulator-version-min=${iphoneos} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
export LDFLAGS="-arch ${ARCH} -miphonesimulator-version-min=${iphoneos} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
make distclean
./configure darwin64-x86_64-cc no-engine no-hw no-shared
make clean
make
cp libssl.a "libssl-${ARCH}.a"
cp libcrypto.a "libcrypto-${ARCH}.a"
build_four="SIM: $ARCH"

# generate fat binary

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

lipo -create libssl-arm64.a libssl-x86_64.a -output libssl.a
lipolog="$(lipo -info libssl.a)"
cp "libssl.a" "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libssl.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

lipo -create libcrypto-arm64.a libcrypto-x86_64.a -output libcrypto.a
lipolog="$(lipo -info libcrypto.a)"
cp "libcrypto.a" "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libcrypto.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################

