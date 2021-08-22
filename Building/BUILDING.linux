=======================================================================
Linux
=======================================================================

The plug-in was built using Code::Blocks svn11417 on Ubuntu 18.04 (64-bit).

Download the FileMaker External Plug-In API from

	http://www.filemaker.com/support/technologies/

and place the FMWrapper directory inside the SDK's Headers directory in the Headers directory. The version of libFMWrapper.so in the Libraries/Linux should be placed the Libraries/linux directory.

The BaseElements plug-in is built as a static library and all third-party libraries must be built from source as static libraries.

In this guide, ${SRCROOT} is the path to the project root (i.e. BaseElements-Plugin) directory.


=======================================================================
Boost - 1.75_0
=======================================================================

The Boost FileSystem and System libraries are required.

	http://www.boost.org/

See

	more/getting_started/index.html

for instructions on compiling the Boost libraries. From the command prompt try

	cd boost_1_75_0
	./bootstrap.sh
	./b2 link=static cflags=-fPIC cxxflags=-fPIC runtime-link=static install --prefix=_build_linux --with-program_options --with-regex --with-date_time --with-filesystem --with-thread


Copy the directory containing the boost headers from the _build_linux/include directory to the Headers directory and copy the files in the _build_linux/lib to Libraries/linux.


=======================================================================
Jansson - 2.5
=======================================================================

Download the source from

	https://github.com/GoyaPtyLtd/jansson

Then

	cd jansson-json_path
	autoreconf -i
	CFLAGS=-fPIC ./configure --prefix=$(pwd)/_build_linux
	make install

Copy jansson.h and jansson_config.h from build/include to the Headers directory and  _build_linux/lib/libjansson.a to Libraries/linux.


======================================================================
zlib - 1.2.8
=======================================================================

Building zlib is straightforward. Download the zlib source from

	http://www.zlib.net/

and extract the folder zlib-1.2.8 to the desktop.

	cd zlib-1.2.8
	CFLAGS="-fPIC" ./configure --static --prefix="$(pwd)/_build_linux"
	make install

Copy the header and library files.

	cp -R _build_linux/include "${SRCROOT}/Headers/zlib"
	cp _build_linux/lib/libz.a "${SRCROOT}/Libraries/linux/"


=======================================================================
OpenSSL - 1.1.1
=======================================================================

Download the latest openssl source from

	http://www.openssl.org/source/

and extract the archive. Then

	cd openssl-OpenSSL_1_1_1-stable 
	./Configure linux-generic64 no-engine no-hw no-shared --prefix="$(pwd)/_build_linux"
	make install

Copy the header and library files.

	cp -R _build_linux/include/openssl "${SRCROOT}/Headers/"
	cp _build_linux/lib/libcrypto.a ./_build_linux/lib/libssl.a "${SRCROOT}/Libraries/linux/"


=======================================================================
libssh2 - 1.9.0
=======================================================================

Notes:
 - zlib and OpenSSL must be built before building libssh2.
 - For v1.9.0 use the "nightly" source archive rather than the release download.
 
Download the source from

	http://www.libssh2.org/

and extract the archive. Then

	cd libssh2-1.9.0
	CFLAGS="-fPIC -I${SRCROOT}/Headers -I${SRCROOT}/Headers/zlib" LDFLAGS="-L${SRCROOT}/Libraries/linux/" LIBS="-ldl" ./configure --disable-shared --disable-examples-build --prefix="$(pwd)/_build_linux"
	make install

Copy the header and library files.

	cp -R _build_linux/include "${SRCROOT}/Headers/libssh2"
	cp _build_linux/lib/libssh2.a "${SRCROOT}/Libraries/linux"


=======================================================================
libcurl - 7.76.1
=======================================================================

Note: zlib, OpenSSL & libssh2 must be built before building libcurl.

Download the source from

	http://curl.haxx.se/download.html

and expand it into a directory on the Desktop. Then

	cd curl-7.76.1
	CPPFLAGS="-I${SRCROOT}/Headers -I${SRCROOT}/Headers/zlib -I${SRCROOT}/Headers/libssh2" LDFLAGS="-L${SRCROOT}/Libraries/linux" LIBS="-ldl" ./configure --disable-dependency-tracking --enable-static --disable-shared --with-ssl --with-zlib --with-libssh2 --without-tests --prefix="$(pwd)/_build_linux"
	make install

Copy the header and library files.

	cp -R _build_linux/include/curl "${SRCROOT}/Headers/"
	cp _build_linux/lib/libcurl.a "${SRCROOT}/Libraries/linux"


=======================================================================
libiconv - 1.16
=======================================================================

Download the source for libiconv from

	https://www.gnu.org/software/libiconv/#downloading

and extract the archive. Then

	cd libiconv-1.16
	CFLAGS=-fPIC ./configure --disable-shared --prefix="$(pwd)/_build_linux"
	make install

Copy the Headers and libraries to the project directory

	cp -R _build_linux/include/*.h "${SRCROOT}/Headers/iconv"
	cp _build_linux/lib/libiconv.a  _build_linux/lib/libcharset.a "${SRCROOT}/Libraries/linux"


=======================================================================
libxml2 - 2.9.10
=======================================================================

Download the source for libxml2 from

	http://xmlsoft.org/downloads.html

and expand it into a directory on the Desktop.

	cd libxml2-2.9.10
	CFLAGS=-fPIC ./configure --disable-shared --with-threads --without-python --without-zlib --without-lzma --prefix="$(pwd)/_build_linux"
	make install

Copy the Headers and libraries to the project directory

	cp -R _build_linux/include/libxml2 "${SRCROOT}/Headers"
	cp _build_linux/lib/libxml2.a "${SRCROOT}/Libraries/linux"


=======================================================================
libxslt & libexslt - 1.1.34
=======================================================================

Download the source from

	http://xmlsoft.org/XSLT/downloads.html

and expand it into a directory on the Desktop.

	cd libxslt-1.1.34
	CFLAGS=-fPIC ./configure --with-libxml-prefix="$(pwd)/../libxml2-2.9.10/_build_linux" --prefix="$(pwd)/_build_linux"
	make install

Finally, place the headers

	cp -R _build_linux/include/libxslt "${SRCROOT}/Headers"
	cp _build_linux/lib/libxslt.a ./_build/lib/libexslt.a "${SRCROOT}/Libraries/Linux"


=======================================================================
duktape - 2.6.0
=======================================================================

Download the source from

	http://duktape.org/

and extract the archive to the desktop and copy the src directory to the Source directory and rename it to duktape.


=======================================================================
libjpeg-turbo - 2.0.6
=======================================================================

Download the source from

	http://www.libjpeg-turbo.org/

and extract the archive. Then

	cd libjpeg-turbo-2.0.6
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX="./_build_linux" -DENABLE_SHARED=NO -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_IGNORE_PATH=/usr/lib/x86_64-linux-gnu/ ./
	make install

Copy the header and library files.

	cp -R _build_linux/opt/libjpeg-turbo/include "${SRCROOT}/Headers/libturbojpeg"
	cp _build_linux/opt/libjpeg-turbo/lib64/libturbojpeg.a "${SRCROOT}/Libraries/linux"


=======================================================================
Poco 1.10.1
=======================================================================

Download the source from

	http://pocoproject.org/download/index.html

Then

	cd poco-master
	./configure --cflags="-std=c++11 -fPIC" --static --no-tests --no-samples --prefix="$(pwd)/_build_linux" --poquito --omit=CppParser,Crypto,Data,Encodings,MongoDB,PDF,PageCompiler,Redis,Util
	make install


	cp -R _build_linux/include/Poco "${SRCROOT}/Headers"
	cp _build_linux/lib/libPocoFoundation.a _build_linux/lib/libPocoNet.a _build_linux/lib/libPocoXML.a _build_linux/lib/libPocoZip.a _build_linux/lib/libPocoJSON.a "${SRCROOT}/Libraries/Linux"

=======================================================================
freetype - 2.10.4
=======================================================================

Download freetype from

	https://www.freetype.org/download.html
	
and expand it into a directory on the Desktop.

	cd freetype-2.10.4
	CFLAGS="-fPIC" ./configure --disable-shared --prefix=$(pwd)/_build_linux
	make install

then copy _build_linux/lib/libfreetype.a to Libraries/linux and _build_linux/include/freetype2 to Headers.


=======================================================================
fontconfig - 2.13.1
=======================================================================

Download fontconfig from

	https://www.freedesktop.org/wiki/Software/fontconfig/
	
and expand it into a directory on the Desktop.

	cd fontconfig-2.13.1
	CFLAGS="-fPIC" ./configure --disable-shared --prefix=$(pwd)/_build_linux FREETYPE_CFLAGS="-I${SRCROOT}/Headers/freetype2" FREETYPE_LIBS="${SRCROOT}/Libraries/linux" LDFLAGS="-L${SRCROOT}/Libraries/linux" --with-expat=/usr/lib64/libexpat.so.1
	make install

then copy _build_linux/lib/libfontconfig.a to Libraries/linux and _build_linux/include/fontconfig to Headers.


=======================================================================
podofo - 0.9.7
=======================================================================

Download podofo from

	http://podofo.sourceforge.net/download.html
	
and expand it into a directory on the Desktop.

	cd podofo-0.9.7
	cmake -G "Unix Makefiles" -DWANT_FONTCONFIG:BOOL=TRUE -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX="./_build_linux" -DPODOFO_BUILD_STATIC:BOOL=TRUE -DPODOFO_BUILD_SHARED:BOOL=FALSE -DFREETYPE_INCLUDE_DIR="${SRCROOT}/Headers/freetype2" -DFREETYPE_LIBRARY_RELEASE="${SRCROOT}/Libraries/linux/libfreetype.a" -DFONTCONFIG_INCLUDE_DIR="${SRCROOT}/Headers/fontconfig" -DFONTCONFIG_LIBRARY_RELEASE="${SRCROOT}/Libraries/linux/libfontconfig.a" -DFONTCONFIG_LIBRARIES="${SRCROOT}/Libraries/linux" -DPODOFO_BUILD_LIB_ONLY=TRUE -DWANT_LIB64:BOOL=TRUE -DCMAKE_CXX_FLAGS="-fPIC" ./
	make install

Copy the header and library files.

	cp -R _build_linux/include/podofo "${SRCROOT}/Headers"
	cp _build_linux/lib64/libpodofo.a "${SRCROOT}/Libraries/linux"

=======================================================================


