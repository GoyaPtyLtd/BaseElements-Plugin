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
cd libjpeg-turbo-2.0.6


########################################################################
# Main
########################################################################

set -e

########################################################################
# Device
########################################################################

ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

IOS_SDK=$(xcrun --sdk iphoneos --show-sdk-path)
cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=${IOS_SDK} -DCMAKE_INSTALL_PREFIX="./_build_ios" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_C_FLAGS="-arch ${ARCH} -miphoneos-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_FLAGS="-arch ${ARCH} -miphoneos-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_STANDARD=11
make install

cp _build_ios/lib/libturbojpeg.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libturbojpeg.a"
cp _build_ios/lib/libjpeg.a "${SRCROOT}/Libraries/iOS/iPhoneOS/libjpeg.a"

build_one="Device: $ARCH"


########################################################################
# Simulator
########################################################################

IOS_SDK=$(xcrun --sdk iphonesimulator --show-sdk-path)

rm CMakeCache.txt
rm -rf CMakeFiles


ARCH="arm64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=${IOS_SDK} -DCMAKE_INSTALL_PREFIX="./_build_iossimulator_${ARCH}" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_C_FLAGS="-arch ${ARCH} -miphonesimulator-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_FLAGS="-arch ${ARCH} -miphonesimulator-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_STANDARD=11
make install

cp "_build_iossimulator_${ARCH}/lib/libturbojpeg.a" "libturbojpeg_${ARCH}.a"
cp "_build_iossimulator_${ARCH}/lib/libjpeg.a" "libjpeg_${ARCH}.a"

build_three="Device: $ARCH"


rm CMakeCache.txt
rm -rf CMakeFiles


ARCH="x86_64"
echo "---> Building ${ARCH}-${iphoneos}" | awk '/'${ARCH}'/ {print "\033[34;25;62m" $0 "\033[0m"}'

cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=${IOS_SDK} -DCMAKE_INSTALL_PREFIX="./_build_iossimulator_${ARCH}" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_C_FLAGS="-arch ${ARCH} -miphonesimulator-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_FLAGS="-arch ${ARCH} -miphonesimulator-version-min=$iphoneos -stdlib=libc++" -DCMAKE_CXX_STANDARD=11
make install

cp "_build_iossimulator_${ARCH}/lib/libturbojpeg.a" "libturbojpeg_${ARCH}.a"
cp "_build_iossimulator_${ARCH}/lib/libjpeg.a" "libjpeg_${ARCH}.a"

build_four="Device: $ARCH"

echo "---> Success: $build_one $build_two $build_three $build_four" | awk '/Success/ {print "\033[37;35;53m" $0 "\033[0m"}'

# argh !!! we already have a fat binary ???

cp _build_iossimulator_x86_64/lib/libturbojpeg.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libturbojpeg.a"
cp _build_iossimulator_x86_64/lib/libjpeg.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libjpeg.a"

rm CMakeCache.txt
rm -rf CMakeFiles
exit

# end argh !!!

# simulator fat binary

lipo -create libturbojpeg_arm64.a libturbojpeg_x86_64.a -output libturbojpeg.a
lipolog="$(lipo -info libturbojpeg.a)"
cp libturbojpeg.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libturbojpeg.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

lipo -create libjpeg_arm64.a libjpeg_x86_64.a -output libjpeg.a
lipolog="$(lipo -info libjpeg.a)"
cp libjpeg.a "${SRCROOT}/Libraries/iOS/iPhoneOSSimulator/libjpeg.a"
echo "---> $lipolog" | awk '/Arch/ {print "\033[32;35;52m" $0 "\033[0m"}'

echo "---> Build Process Complete!" | awk '/!/ {print "\033[36;35;54m" $0 "\033[0m"}'

rm CMakeCache.txt
rm -rf CMakeFiles


########################################################################
# End
########################################################################

