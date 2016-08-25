/*
 BEQuadChar.h
 BaseElements Plug-In

 Copyright 2015-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#ifndef __BaseElements__BEQuadChar__
#define __BaseElements__BEQuadChar__


#include "BEPluginGlobalDefines.h"


#include <string>


#define MAIN_CONTAINER_TYPE "MAIN"
#define FILE_CONTAINER_TYPE "FILE"
#define COMPRESSED_CONTAINER_TYPE "ZLIB"

#define SOUND_CONTAINER_TYPE "snd "
#define DPI_CONTAINER_TYPE "DPI_"
#define FILENAME_CONTAINER_TYPE "FNAM"
#define SIZE_CONTAINER_TYPE "SIZE"

#define JPEG_CONTAINER_TYPE "JPEG"
#define GIF_CONTAINER_TYPE "GIFf"
#define EPS_CONTAINER_TYPE "EPS "
#define META_CONTAINER_TYPE "META"
#define PNG_CONTAINER_TYPE "PNGf"
#define BMP_CONTAINER_TYPE "BMPf"
#define PDF_CONTAINER_TYPE "PDF "


class BEQuadChar {


	public:

		BEQuadChar ( const std::string new_type );
		BEQuadChar ( const fmx::BinaryData& data, const fmx::int32 which );

		const bool is_file ( ) const;
		const bool is_zlib ( ) const;
		const bool is_jpeg ( ) const;
		const bool is_image ( ) const;
		const bool is_image_attribute ( ) const;

		fmx::QuadCharUniquePtr get_type ( );

	private:

		const bool is_type ( const std::string new_type ) const;

		fmx::QuadCharUniquePtr type;

};

#endif /* defined(__BaseElements__BEQuadChar__) */

