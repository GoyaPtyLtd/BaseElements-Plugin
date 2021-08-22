=======================================================================
Windows
=======================================================================

The plug-in was built using Visual Studio 2019 Professional on Windows 10.

Download the FileMaker External Plug-In API from

	http://www.filemaker.com/support/technologies/

and place the FMWrapper directory inside the SDK's Headers directory in the Headers directory. The version of FMWrapper.lib in the Libraries\win64 should be placed the Libraries directory.

The BaseElements plug-in is built as a static library and all third-party libraries must be built from source as static libraries (/MTd & /MT) using libcmt.lib and nafxcw.lib for release and libcrtd.lib and nafxcwd.lib for debug builds.

Unicode is used rather than the multi-byte character set.

Note: source code for some of the libraries is supplied as gzipped tar archives; there are a number of applications that can unpack these archives, for example, Stuffit Expander

	http://www.stuffit.com/win-expander.html

Another note: when doing both debug and release builds clean the project when switching from one t'other.


=======================================================================
Boost - 1.75_0
=======================================================================

The Boost FileSystem and System libraries are required.

	http://www.boost.org/

See

	more/getting_started/index.html

for instructions on compiling the Boost libraries. From the command prompt try

	cd boost_1_75_0
	bootstrap.bat
	b2 address-model=64 link=static runtime-link=static --with-program_options --with-thread --with-filesystem --with-date_time --with-chrono --with-regex --prefix=build install

Copy the directory containing the boost headers from the build/include directory to the Headers directory and copy the files in the build/lib to Libraries/win64.


=======================================================================
zlib
=======================================================================

Building zlib is straightforward. Download the zlib source from

	http://www.zlib.net/

and extract the folder zlib-1.2.8 to the desktop.

Open the solution file "contrib/vstudio/vc11/zlibvc.sln" and remove ZLIB_WINAPI from the list of preprocessor definitions for the libzstat x64 ReleaseWithoutAsm configuration. From the "Build Menu" select "Batch Build" and select the libzstat "ReleaseWithoutAsm" configuration for x64, and "Build".

Move

	contrib/vstudio/vc11/x64/ZlibStatReleaseWithoutAsm/zlibstat.lib

into the Libraries/win64.

Copy zip.h and zconf.h from the root directory of the distribution to the Headers directory.


=======================================================================
Jansson - 2.5
=======================================================================

Download the source from

	https://github.com/rogerz/jansson/tree/json_path

Download the source from

	https://github.com/GoyaPtyLtd/jansson

And build the release configuration.

Copy jansson.h and jansson_config.h from ./src/ to the Headers directory and win32/vs2010/Output/Release/jansson.lib to Libraries\win64.


=======================================================================
OpenSSL - 1.1.1
=======================================================================

Building OpenSSL requires that perl is installed. The ActiveState perl (Community Edition) is a good
place to start if you do not already have perl installed

	http://www.activestate.com/activeperl


Download the latest openssl source from

	http://www.openssl.org/source/

Unpack the archive and, in a "VS2019 x64 Native Command Prompt":

	cd openssl-OpenSSL_1_1_1-stable 
	perl Configure VC-WIN64A enable-static-engine no-asm
	nmake
	
	
	ms\do_win64a
	nmake -f ms\nt.mak

Copy libcrypto_static.lib and libssl_static.lib to Libraries\win64 and copy include\openssl to Headers.


=======================================================================
libssh2 - 1.9.0
=======================================================================

Download the source from

	http://www.libssh2.org/

Open

	win32\libssh2.dsw

and set the following Configuration Properties for OpenSSL Lib Release:

	Project Defaults -> Character Set == Use Unicode Character Set
	VC++ Directories -> Include Directories -> add the path to the BEPlugin Headers directory
	C/C++ -> Code Generation -> Runtime Library == Multi-threaded (/MT)

Comment out the line

	#define snprintf _snprintf

in libssh2_config.h.

Select <New…> in the "Active Solution Platform" pop-up in the "Configuration Manager". Then select x64 in "Type or select the new platform", "Copy settings from" should be Win32 and check "Create New project platforms" and click "OK".

Build the library and copy it from

	win32\Release_lib\libssl2.lib

into the Libraries\win64 directory and copy

	include

to the Headers directory and rename it libssh2.


=======================================================================
libcurl - 7.76.1
=======================================================================

Note: OpenSSL & libssh2 must be built before building libcurl.

Download the source from

	http://curl.haxx.se/download.html

and expand it into a directory on the Desktop.

Open

	projects\Windows\VC15\curl-all.sln

and set the following Configuration Properties for the libssh "LIB Release - LIB OpenSSL - LIB LibSSH2" configuration:

	VC++ Directories -> Include Directories -> add the path to the BEPlugin Headers/libssh directory
	VC++ Directories -> Include Directories -> add the path to the BEPlugin Headers directory
	VC++ Directories -> Library Directories -> add the path to the BEPlugin Libraries/win64 directory
	C/C++ -> Code Generation -> Runtime Library == Multi-threaded (/MT)

In Visual Studio and select "Batch Build" and select the x64 "LIB Release - LIB OpenSSL - LIB LibSSH2" configuration and "Build" it.

Copy the library from

	build\Win64\VC15\LIB Release - LIB OpenSSL - LIB LibSSH2\libcurl.lib

into the Libraries\win64 directory and copy

	include\curl

to the Headers directory.


=======================================================================
liboauth - 1.0.3
=======================================================================

Note: OpenSSL should be built first.

Download the source from

	http://liboauth.sourceforge.net/

Unpack directory containing the Visual Studio solution files (from the BaseElements-Plugin Projects/Extras), open the Solution make sure the project can find the BaseElements plug-in Header and Libraries directories and then the "Release" configuration.

Copy liboauth.h from ./src/ to the Headers directory and liboauth\Release\liboauth.lib to Libraries\win64.


=======================================================================
libiconv - 1.16
=======================================================================

Download the libiconv source from

	https://github.com/kiyolee/libiconv-win-build
	
open

	build-VS2019-MT/libiconv.sln

set

	Configuration Properties -> C/C++ -> Optimization -> Whole Program Optimization
	
to

	No
	
Build the libiconv-static, Release, x64 configuration and copy

	inlcude\iconv.h
	
to

	Headers\iconv\iconv.h
	
and the library from

	build-VS2019-MT\x64\Release\libiconv-static.lib

to

	Libraries\win64\iconv.lib
		
		
=======================================================================
libxml2 - 2.9.10
=======================================================================

Download the source from

	https://gitlab.gnome.org/GNOME/libxml2/

and expand it into a directory on the Desktop.


Launch the "VS2019 x64 Native Command Prompt" then

	cd Desktop\libxml2-2.9.10\win32

Run the commands (changing the include= & lib= paths as appropriate)

	cscript configure.js static=yes debug=no cruntime=-MT include="<path to BaseElements-Plugin\Headers\iconv>" lib="<patjh tp BaseElements-Plugin>\Libraries\win64"

Then

	nmake -f Makefile.msvc MFLAGS=-MT

to configure and build the source.

Finally, copy the headers directory

	include\libxml2

into the project Headers directory and copy the library

	bin.msvc\libxml2_a.lib

into the project Libraries\win64 directory as

	libxml2.lib


=======================================================================
libxslt - 1.1.34
=======================================================================

Download the source from

	https://gitlab.gnome.org/GNOME/libxslt/

and expand it into a directory on the Desktop.


Launch the "VS2019 x64 Native Command Prompt" then

	cd Desktop\libxslt-1.1.34\win32

Run the commands (changing the include= & lib= paths as appropriate)

	cscript configure.js static=yes debug=no cruntime=-MT include="<path to BaseElements-Plugin\Headers>;<path to BaseElements-Plugin\Headers\iconv>" lib="<patjh tp BaseElements-Plugin>\Libraries\win64"

Then

	nmake -f Makefile.msvc MFLAGS=-MT libxslta libexslta

to configure and build the source.

Finally, place the headers from the directories

	libxslt-1.1.34\libxslt & libxslt-1.1.34\libexslt

into directories, libxslt & libexslt, in the Headers directory and copy the libraries

	bin.msvc\libxslt_a.lib & bin.msvc\libexslt_a.lib

into the Libraries\win64 directory.


=======================================================================
duktape - 2.6.0
=======================================================================

Download the source from

	http://duktape.org/

and extract the archive to the desktop and copy the src directory to the Source directory and rename it to tape.


=======================================================================
libjpeg-turbo - 2.0.6
=======================================================================

To build first download and install NASM from

	http://www.nasm.us/

Download the source from

	http://www.libjpeg-turbo.org/

and extract the archive to the desktop. Download and install cmake from

	 http://www.cmake.org/download/

and then, from that directory, in a terminal window

	cmake -G "Visual Studio 14 Win64"

Open the project

	turbojpeg-static

and in

	Configuration Properties -> General -> Project Defaults

set

	Character Set = Use Unicode Character Set

and build the Release configuration and copy turbojpeg-static.lib to the Liraries/win64 directory.

Create a directory libturbojpeg in the Headers directory and copy

	jconfig.h
	jerror.j
	jmorecfg.h
	jpeglib.h
	turbo.jpg.h

into that from the source directory.


=======================================================================
Poco 1.10.1
=======================================================================

Download the source from

	http://pocoproject.org/download/index.html

Then, after extracting the archive set

	Configuration Properties -> General -> Project Defaults -> Character Set = Use Unicode Character Set

Then build the Zip, Util, Net, JSON and Foundation MT Release and Debug libraries using the projects in the respective directories.

Copy PocoFoundationmt.lib/PocoFoundationmtd.lib, libPocoJSON.a/libPocoJSONd.a, libPocoNet.a/libPocoNetd.a, PocoXMLmt.lib/PocoXMLmtd.lib & PocoZipmt.lib/PocoZipmtd.lib libraries from the lib and lib64 driectories into the Libraries/win32 and Libraries/win64 directory and copy the include/Poco directory to Headers.


=======================================================================
freetype - 2.10.4
=======================================================================

Download freetype from

	https://www.freetype.org/download.html
	
and expand it into a directory on the Desktop.

Open the project file at

	builds/windows/vc2010
	
and build the x64 Release Static configuration.

then copy objs/x64/Release Static/freetype.lib to Libraries/Win64 and rename include to freetype2 and copy to Headers.


=======================================================================
podofo - 0.9.7
=======================================================================

Download podofo from

	http://podofo.sourceforge.net/download.html
	
and expand it into a directory on the Desktop.

	set SRCROOT=<the path to the plugin directory>
	
	cd podofo-0.9.7
	cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX="_build_windows" -DPODOFO_BUILD_STATIC:BOOL=TRUE -DPODOFO_BUILD_SHARED:BOOL=FALSE -DPODOFO_BUILD_LIB_ONLY=TRUE -DCMAKE_INCLUDE_PATH=%SRCROOT%\Headers -DCMAKE_LIBRARY_PATH=%SRCROOT%\Libraries\Win64 -DZLIB_LIBRARY_RELEASE=%SRCROOT%\Libraries\Win64\libstat.lib -DWANT_LIB64:BOOL=TRUE -DFREETYPE_INCLUDE_DIR="%SRCROOT%\Headers\freetype2" -DFREETYPE_LIBRARY_RELEASE="%SRCROOT%\Libraries\Win64\freetype27.lib" ./

Open the ALL_BUILD project and for the "podofo-static" release target set the Configuration Properties

	Advanced -> Character Set == Use Unicode Character Set
	C/C++ -> Code Generation -> Runtime Library == Multi-threaded (/MT)
	
For the debug target set

	General -> Target Name -> podofod
	Advanced -> Character Set == Use Unicode Character Set
	C/C++ -> Code Generation -> Runtime Library == Multi-threaded (/MTd)


then batch build the INSTALL targets.

Copy the header and library files.

	copy _build_windows\include\podofo "${SRCROOT}\Headers"
	copy _build_windows\lib64\podofo.lib _build_windows\lib64\podofod.lib %SRCROOT%\Libraries\Win64"


=======================================================================

