/* Template filled out by CMake */

/*
 * *** THIS HEADER IS INCLUDED BY PdfCompilerCompat.h ***
 * *** DO NOT INCLUDE DIRECTLY ***
 */
#ifndef _PDF_COMPILERCOMPAT_H
#error Please include PdfDefines.h instead
#endif

#define PODOFO_VERSION_MAJOR 0
#define PODOFO_VERSION_MINOR 9
#define PODOFO_VERSION_PATCH 7

/* PoDoFo configuration options */
#define PODOFO_MULTI_THREAD

/* somewhat platform-specific headers */
#define PODOFO_HAVE_UNIQUE_PTR 1
#define PODOFO_HAVE_STRINGS_H 1
#define PODOFO_HAVE_ARPA_INET_H 1
/* #undef PODOFO_HAVE_WINSOCK2_H */
/* #undef PODOFO_HAVE_MEM_H */
#define PODOFO_HAVE_CTYPE_H 1

/* Integer types - headers */
#define PODOFO_HAVE_STDINT_H 1
/* #undef PODOFO_HAVE_BASETSD_H */
#define PODOFO_HAVE_SYS_TYPES_H 1
/* Integer types - type names */
#define PDF_INT8_TYPENAME   int8_t
#define PDF_INT16_TYPENAME  int16_t
#define PDF_INT32_TYPENAME  int32_t
#define PDF_INT64_TYPENAME  int64_t
#define PDF_UINT8_TYPENAME  uint8_t
#define PDF_UINT16_TYPENAME uint16_t
#define PDF_UINT32_TYPENAME uint32_t
#define PDF_UINT64_TYPENAME uint64_t

/* Sizes of int64 and long, to pick proper printf format */
#define SZ_INT64 8
#define SZ_LONG 8

/* Endianness */
/* #undef TEST_BIG */

/* Features */
/* #undef PODOFO_NO_FONTMANAGER */

/* Libraries */
/* #undef PODOFO_HAVE_JPEG_LIB */
/* #undef PODOFO_HAVE_PNG_LIB */
/* #undef PODOFO_HAVE_TIFF_LIB */
#define PODOFO_HAVE_FONTCONFIG
/* #undef PODOFO_HAVE_LUA */
/* #undef PODOFO_HAVE_BOOST */
/* #undef PODOFO_HAVE_CPPUNIT */
/* #undef PODOFO_HAVE_OPENSSL */
/* #undef PODOFO_HAVE_OPENSSL_1_1 */
/* #undef PODOFO_HAVE_OPENSSL_NO_RC4 */
/* #undef PODOFO_HAVE_LIBIDN */
/* #undef PODOFO_HAVE_UNISTRING_LIB */
