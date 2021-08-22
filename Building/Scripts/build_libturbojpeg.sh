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

mkdir libjpeg-turbo
cd libjpeg-turbo

mkdir libs

rm -rf src
cd src

autoreconf -fi

IOS_PLATFORMDIR=$(xcrun --sdk iphoneos --show-sdk-platform-path)
IOS_SYSROOT=$(xcrun --sdk iphoneos --show-sdk-path)
IOS_SIMULATOR_PLATFORMDIR=$(xcrun --sdk iphonesimulator --show-sdk-platform-path)
IOS_SIMULATOR_SYSROOT=$(xcrun --sdk iphonesimulator --show-sdk-path)
IOS_GCC=$(/usr/bin/xcode-select -p)/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang

## arm64
echo "--- Building for arm64 ---"
IOS_CFLAGS="-arch arm64 -miphoneos-version-min=8.0"
export CFLAGS="-Wall -isysroot $IOS_SYSROOT -O3 $IOS_CFLAGS -fembed-bitcode -funwind-tables"
export ASMFLAGS=""

cat <<EOF >toolchain.cmake
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang)
EOF

cmake -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake \
  -DCMAKE_OSX_SYSROOT=${IOS_SYSROOT[0]} \
  .
make
mv -v ./libturbojpeg.a ../libs/libturbojpeg_arm64_a

cd ..
rm -rf src
git clone https://github.com/libjpeg-turbo/libjpeg-turbo.git src
cd src

##  x86_64
echo "--- Building for x86_64 ---"
IOS_CFLAGS="-arch x86_64 -miphonesimulator-version-min=8.0"
export CFLAGS="$IOS_CFLAGS -isysroot $IOS_SIMULATOR_SYSROOT"
export ASMFLAGS=""
export LDFLAGS=""
export NASM=/usr/local/bin/nasm

cat <<EOF >toolchain.cmake
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang)
EOF

cmake -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake \
  -DCMAKE_OSX_SYSROOT=${IOS_SIMULATOR_SYSROOT[0]} \
  .
make
mv -v ./libturbojpeg.a ../libs/libturbojpeg_x86_64_a

## lipo
mkdir -p ../libs/ios/device
mkdir -p ../libs/ios/simulator
mkdir -p ../libs/ios/universal

# Simulator fat binary
xcrun -sdk iphoneos lipo -arch i386 ../libs/libturbojpeg_x86_a -arch x86_64 ../libs/libturbojpeg_x86_64_a -create -output ../libs/ios/simulator/libturbojpeg.a

cd ..
rm -rf src

########################################################################
# End
########################################################################

