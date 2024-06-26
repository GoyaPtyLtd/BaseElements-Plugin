###Building

The BE plugin consist of three parts : the plugin SDK from Claris, pre-built code from the open source libraries we use, and the BE code itself.

Whenn downloading the plugin from github, you must use a version of git that supports git-lfs.

    If you're unfamiliar with git-lfs, we recommend https://desktop.github.com as it will handle that for you.
    We also recommend you start by cloning the branch you want to build directly as there are a lot of files to download
    and that will save you switching branches to download a second time.

###Plugin SDK

The SDK comes from Claris and to get the latest version, download the FileMaker External Plug-In API from the link at the very bottom of :

    https://www.claris.com/resources/downloads/

Mac : place the FMWrapper.framework in the BaseElements-Plugin/Libraries/macOS directory.

Windows : place the FMWrapper directory inside the SDK's Headers directory in the Headers directory. The version of FMWrapper.lib in the Libraries\win64 should be placed the Libraries directory.

###Pre Built Open Source Libraries

So you do not need to compile the individual libraries to compile the plugin. Libraries change all the time, so we would love for help to build new release versions of these libraries. If you'd like to help with that, there is a separate repo for all the libraries:

https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries

###BE Plugin specific code

All changes to code are submitted to the Development branch which should always compile and pass the tests. From there new releases are made which go to the main branch.

###Building the plugin

The code uses Xcode on the mac, Visual Studio on Windows and CodeBlocks on Linux. ( CodeBlocks is launched with sudo, and although this should not be necessary it does seem to solve a lot of other issues. )

You will need your own certificates for Mac or Win versions.

You should have FileMaker Pro installed before compiling as the project puts the final version in the Extensions folder to start testing with. ( install FMS on linux instead ).

###macOS

Once you've cloned the branch you want, then you will need to configure the code signing in Xcode.

Open the Project/BaseElements.xcodeproj file in Xcode. Choose a target and compile for that target.

###Windows

The plug-in was built using Windows 11 Professional and Visual Studio 2022.

The BaseElements plug-in is built as a static library and all third-party libraries must be built from source as static libraries (/MTd & /MT) using libcmt.lib and nafxcw.lib for release and libcrtd.lib and nafxcwd.lib for debug builds.

Unicode is used rather than the multi-byte character set.

Another note: when doing both debug and release builds clean the project when switching from one t'other.

###Linux

The plug-in was built using Code::Blocks 20.03 on Ubuntu 22.04.3 (64-bit). The BaseElements plug-in is built as a static library ( all third-party libraries are built from source as static libraries ).

To get setup on a fresh ubuntu install :

    sudo apt update
    sudo apt upgrade
    sudo apt install git-all git-lfs
    sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"

    sudo apt install codeblocks cmake gperf libc++-dev libc++abi-dev libexpat1-dev lld lldb liblldb-dev libomp5 libomp-dev llvm llvm-dev llvm-runtime libllvm-ocaml-dev clang clangd clang-format clang-tidy clang-tools clang libclang-dev libclang1 python3-clang

Install FileMaker Server first. For example ( replace these links with the latest version and the correct platform ).

**Ubuntu 22 ARM**

    wget https://downloads.claris.com/esd/fms_20.1.2.207_Ubuntu22_arm64.zip
    unzip fms_20.1.2.207_Ubuntu22_arm64.zip
    sudo apt install ./filemaker-server-20.1.2.207-arm64.deb

Then download the plugin and open codeblocks to compile :

    mkdir ~/source
    cd ~/source
    git clone https://github.com/GoyaPtyLtd/BaseElements-Plugin.git

    sudo codeblocks

( sudo possibly not required, but at the moment solves a tonne of issues )

Use llvm clang as the default compiler.
