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
cd libxslt-1.1.34

########################################################################
# Main
########################################################################

set -e

export CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
# export AR="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar"

########################################################################
# Device
########################################################################

IOS_SDK=$(xcrun --sdk iphoneos --show-sdk-path)
export LIBXML_CFLAGS="-I${IOS_SDK}/usr/include"
export LIBXML_LIBS="-L${IOS_SDK}/usr/lib -lxml2 -lz -lpthread -licucore -lm"


ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -I${SRCROOT}/Headers -isysroot ${IOS_SDK} -miphoneos-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -L${SRCROOT}/Libraries/iOS/iPhoneOS -isysroot ${IOS_SDK} -miphoneos-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --target="arm64-apple-ios" --disable-shared --enable-static --without-python --without-crypto


 # -I${IOS_SDK}/usr/include/libxml2
 # -L${IOS_SDK}/usr/lib
 # exit

make clean
make

cp libxslt/.libs/libxslt.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libxslt.a"
cp libexslt/.libs/libexslt.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libexslt.a"

build_one="$ARCH"


########################################################################
# Simulator
########################################################################

IOS_SDK=$(xcrun --sdk iphonesimulator --show-sdk-path)
export LIBXML_CFLAGS="-I${IOS_SDK}/usr/include"
export LIBXML_LIBS="-L${IOS_SDK}/usr/lib -lxml2 -lz -lpthread -licucore -lm"


ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -I${SRCROOT}/Headers -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -L${SRCROOT}/Libraries/iOS/iPhoneOSSimulator -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --disable-shared --enable-static --without-python --without-crypto
make clean
make

cp libxslt/.libs/libxslt.a "libxslt-${ARCH}.a"
cp libexslt/.libs/libexslt.a "libexslt-${ARCH}.a"

build_three="$ARCH"


ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

export CFLAGS="-arch ${ARCH} -I${SRCROOT}/Headers -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
export LDFLAGS="-arch ${ARCH} -L${SRCROOT}/Libraries/iOS/iPhoneOSSimulator -isysroot ${IOS_SDK} -miphonesimulator-version-min=$iphoneos"
./configure --host="aarch64-apple-darwin" --disable-shared --enable-static --without-python --without-crypto
make clean
make

cp libxslt/.libs/libxslt.a "libxslt-${ARCH}.a"
cp libexslt/.libs/libexslt.a "libexslt-${ARCH}.a"

build_four="$ARCH"


echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

lipo -create libxslt-arm64.a libxslt-x86_64.a -output libxslt.a
lipolog="$(lipo -info libxslt.a)"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'
cp "libxslt.a" "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libxslt.a"

lipo -create libexslt-arm64.a libexslt-x86_64.a -output libexslt.a
lipolog="$(lipo -info libexslt.a)"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'
cp "libexslt.a" "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libexslt.a"


echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

########################################################################
# End
########################################################################

