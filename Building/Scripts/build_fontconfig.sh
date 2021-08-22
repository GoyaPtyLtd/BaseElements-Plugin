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
BUILD_DIR=$HOME/Downloads/Fontconfig_iOS_Release # RELEASE_DIR

########################################################################
# Main
########################################################################

mkdir -p ${BUILD_DIR}
cd $BUILD_DIR
cd "../fontconfig-2.13.1"

set -e

iphoneos="10.0"
export CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"

ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -pipe -mdynamic-no-pic -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$iphoneos -I/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/include/libxml2 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk"
export AR="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar"
export LDFLAGS="-arch ${ARCH} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk -miphoneos-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --without-tests --disable-shared --enable-shared=no --enable-static=yes FREETYPE_CFLAGS="-I${SRCROOT}/Headers/freetype2" FREETYPE_LIBS="-L${SRCROOT}/Libraries/iOS -lfreetype" 
make clean
make
cp src/.libs/libfontconfig.a "${BUILD_DIR}/libfontconfig-${ARCH}.a"
build_one="$ARCH"
make distclean

ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'


export CFLAGS="-arch ${ARCH} -pipe  -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$iphoneos -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk"
export LDFLAGS="-static -arch ${ARCH} -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk -miphoneos-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --enable-shared=no --enable-static=yes FREETYPE_CFLAGS="-I${SRCROOT}/Headers/freetype2" FREETYPE_LIBS="-L${SRCROOT}/Libraries/iOS" LDFLAGS="-L${SRCROOT}/Libraries/iOS -lfreetype"
make clean
make
cp src/.libs/libfontconfig.a "${BUILD_DIR}/libfontconfig-${ARCH}.a"
build_four="$ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

lipo -create "${BUILD_DIR}/libfontconfig-arm64.a" "${BUILD_DIR}/libfontconfig-x86_64.a" -output "${BUILD_DIR}/libfontconfig.a"
lipolog="$(lipo -info ${BUILD_DIR}/libfontconfig.a)"

echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'
echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################

