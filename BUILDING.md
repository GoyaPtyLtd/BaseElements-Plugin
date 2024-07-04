## Building

The BE plugin consist of three parts : the plugin SDK from Claris, pre-built code from the open source libraries we use, and the BE code itself.

When downloading the plugin from GitHub, you must use a version of git that supports git-lfs.

    If you're unfamiliar with git-lfs, we recommend https://desktop.github.com as it will handle that for you.
    We also recommend you start by cloning the branch you want to build directly as there are a lot of files to download
    and that will save you switching branches to download a second time.

### Plugin SDK

The SDK comes from Claris and to get the latest version, download the FileMaker External Plug-In API from the link at the very bottom of :

[https://www.claris.com/resources/downloads/]

Mac : place the FMWrapper.framework in the BaseElements-Plugin/Libraries/macOS directory.

iOS : Needs to be documented.

Windows : place the FMWrapper directory inside the SDK's Headers directory in the Headers directory. The version of FMWrapper.lib in the Libraries\win64 should be placed the Libraries directory.

Ubuntu : Needs to be documented - TODO.

## Pre Built Open Source Libraries

So you do not need to compile the individual libraries to compile the plugin. All the required built versions of the libraries are included in this repo.

The code for the kibraries changes all the time, so any time we want to add an updated library, we need to recompile it for all the platforms ( Mac, iOS, iOS Simulator, Windows, Ubuntu 20 x86, Ubuntu 22 x86, Ubuntu 22 ARM ) adn then bring all that new compiled code into this repo. Obvioulsy that's a lot of work, but we don't want that work to prevent people from trying to build their own version of the BE plugin, so they are provided compiled at the most recent version.

However, if you'd like to help us with compiling the libraries as well, there is now a separate repo for all the libraries. As of this writing the work to automat the compiling is only partially complete, so if you can assist we'd love your help.

https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries

These libraries are a rapid work in progress, see the [WorkInProgress](WorkInProgress.md) document in the Development branch for the current state of play, and where to help out.

## BE Plugin specific code

All changes to code are submitted to the Development branch which should always compile and pass the tests. From there new releases are made which go to the main branch.

If you want to submit a change to the plugin, the best way to do that is to fork the repo, make and test your changes and then create a pull request.

## Building the plugin

The code uses Xcode on the Mac, Visual Studio on Windows and CodeBlocks on Linux. ( CodeBlocks is launched with sudo, and although this should not be necessary it does seem to solve a lot of other issues. )

You will need your own certificates for Mac or Win versions, you can get a Mac Developer certificate from Apple, and the Windows Certificates from most certificate providers. You don't need the certificate to compile and test if you authorise the plugin to run in FileMaker Pro.

You should have FileMaker Pro installed before compiling as the project puts the final version in the Extensions folder to start testing with. ( install FMS on linux instead ).

### macOS

Once you've cloned the branch you want, then you will need to configure the code signing in Xcode with your developer account. Open the Project/BaseElements.xcodeproj file in Xcode. Choose a target and compile for that target.

### Windows

The plug-in was built using Windows 11 Professional and Visual Studio 2022.

Some notes for windows :

- The BaseElements plug-in is built as a static library and all third-party libraries must be built from source as static libraries (/MTd & /MT) using libcmt.lib and nafxcw.lib for release and libcrtd.lib and nafxcwd.lib for debug builds.
- Unicode is used rather than the multi-byte character set.
- When doing both debug and release builds clean the project when switching from one tp the other.

### Linux

The plug-in was built using Code::Blocks 20.03 on Ubuntu 22.04.3 (64-bit). The BaseElements plug-in is built as a static library ( all third-party libraries are built from source as static libraries ).

To get setup on a fresh ubuntu install :

    sudo apt update
    sudo apt upgrade
    sudo apt install git-all git-lfs
    sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"

    sudo apt install codeblocks cmake gperf libc++-dev libc++abi-dev libexpat1-dev lld lldb liblldb-dev libomp5 libomp-dev llvm llvm-dev llvm-runtime libllvm-ocaml-dev clang clangd clang-format clang-tidy clang-tools clang libclang-dev libclang1 python3-clang

**Install FileMaker Server first.**

Then download the plugin and open codeblocks to compile :

    mkdir ~/source
    cd ~/source
    git clone https://github.com/GoyaPtyLtd/BaseElements-Plugin.git

    sudo codeblocks

( sudo possibly not required, but at the moment solves a tonne of issues )

Use llvm clang as the default compiler.

This likely needs a lot more documentation about which settings to use.
