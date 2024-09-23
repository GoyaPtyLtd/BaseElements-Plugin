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

## Open Source Libraries

The code for the libraries changes all the time, so any time we want to add an updated library, we need to recompile it for all the platforms ( Mac, iOS, iOS Simulator, Windows, Ubuntu 20 x86, Ubuntu 22 x86, Ubuntu 22 ARM ) and then bring all that new compiled code into this repo.

If you'd like to help us with compiling the libraries, there is now a separate repo for them. As of this writing the work to automate the compiling is only partially complete, so if you can assist we'd love your help.

https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries

These libraries are a rapid work in progress, see the [WorkInProgress](WorkInProgress.md) document in the Development branch for the current state of play, and where to help out.

## BE Plugin specific code

All changes to code are submitted to the Development branch which should always compile and pass the tests. From there new releases are made which go to the main branch.

If you want to submit a change to the plugin, the best way to do that is to fork the repo, make and test your changes and then create a pull request.

## Building the plugin

The code uses Xcode on the Mac, Visual Studio on Windows and cmake on Linux.

You will need your own certificates for Mac or Win versions, you can get a Mac Developer certificate from Apple, and the Windows Certificates from most certificate providers. You don't need the certificate to compile and test if you authorise the plugin to run in FileMaker Pro.

You should have FileMaker Pro and/or Server installed before compiling as the project puts the final version in the Extensions folder to start testing with.  You need FMS installed on linux.

### macOS

Once you've cloned the branch you want, then you will need to configure the code signing in Xcode with your developer account. Open the Project/BaseElements.xcodeproj file in Xcode. Choose a target and compile for that target.

### Windows

The plug-in was built using Windows 11 Professional and Visual Studio 2022.

Some notes for windows :

- The BaseElements plug-in is built as a static library and all third-party libraries must be built from source as static libraries (/MTd & /MT) using libcmt.lib and nafxcw.lib for release and libcrtd.lib and nafxcwd.lib for debug builds.
- Unicode is used rather than the multi-byte character set.
- When doing both debug and release builds clean the project when switching from one tp the other.

### Linux Setup

The BaseElements plug-in is built as a static library ( all third-party libraries are built from source as static libraries ).

To get setup on a fresh ubuntu install :

    sudo apt update
    sudo apt upgrade
    sudo apt install zip

Then install FileMaker Server : 

**For Ubuntu 20**
    wget https://downloads.claris.com/esd/fms_21.0.2.202_Ubuntu20_amd64.zip
    unzip fms_21.0.2.202_Ubuntu20_amd64.zip
    
**For Ubuntu 22 x86**
    wget https://downloads.claris.com/esd/fms_21.0.2.202_Ubuntu22_amd64.zip
    unzip fms_21.0.2.202_Ubuntu22_amd64.zip

**For Ubuntu 22 arm**
    wget https://downloads.claris.com/esd/fms_21.0.2.202_Ubuntu22_arm64.zip
    unzip fms_21.0.2.202_Ubuntu22_arm64.zip

Then install FMS : 

    sudo apt install ./filemaker-server-21.0.2.202-amd64.deb

Install other required software :

    sudo apt install build-essential gperf cmake
    sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"

Grab the repos from GitHub : 
 
    cd ~
    mkdir source
    cd source
    git clone https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries.git
    git clone --depth 1 --branch development https://github.com/GoyaPtyLtd/BaseElements-Plugin.git

As a one off, on ubuntu 20, you need to reconfigure clang so that the command line tools can find the correct binaries.

    cd BaseElements-Plugin-Libraries/scripts/install
    sudo ./update-alternatives-clang.sh

If you didn't previously build the libraries or otherwise download a prebuilt library package :

    cd ~/source/BaseElements-Plugin-Libraries/Scripts
    ./1_getSource.sh                                # Only need to do this once
    ./2_build.sh

### Linux Build

    cd ~/source/BaseElements-Plugin

    mkdir build
    cd build

# Add -DPRO=1 to the cmake config for Pro version.

    cmake -DCMAKE_BUILD_TYPE=Release ..             

    make -j$(($(nproc)+1))
    sudo make install

The make install will put a copy of the BE plugin into the correct Extensions folder.  You need to open the FMS Admin for this machine, and enable Script Engine plugins on the Connectors tab.  Sometimes you may need to restart fmse after installing a new build : 

    fmsadmin restart fmse

And to build a second time, you may need to disable the old plugin, before deleting it via : 

    sudo rm "/opt/FileMaker/FileMaker Server/Database Server/Extensions/./BaseElements.fmx"

And then restarting fmse again to clear it out.