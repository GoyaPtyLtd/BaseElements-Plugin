/*
 BEJPEG.cpp
 BaseElements Plug-In

 Copyright 2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEJPEG.h"

#include "BEPluginException.h"
#include "BEPluginGlobalDefines.h"
#include "BEQuadChar.h"

#include <cmath> // round
#include <iostream>
#include <string>
#include <vector>

#include <libturbojpeg/turbojpeg.h>



BEJPEG::BEJPEG ( )
{
	compression_level = 75;
}


void BEJPEG::set_compression_level ( const int level )
{
	if ( 1 > level ) {
		compression_level = 1;
	} else if ( 100 < level ) {
		compression_level = 100;
	} else {
		compression_level = level;
	}
}


const std::string BEJPEG::get_type ( void )
{
	return JPEG_CONTAINER_TYPE;
}


void BEJPEG::recompress ( void )
{
	decompress();
	compress();
}


void BEJPEG::adjust_dimensions ( const int image_width, const int image_height )
{
	int number_of_scaling_factors = 0;
	tjscalingfactor * scaling_factor = tjGetScalingFactors ( &number_of_scaling_factors );

	if ( width < 1 && height < 1 ) {
		width = image_width;
		height = image_height;
	} else if ( width > 0 && height < 1 ) {
		height = ceil ( ((double)width / (double)image_width) * image_height );
	} else if ( width < 1 && height > 0 ) {
		width = ceil ( ((double)height / (double)image_width) * image_height );
	}

	for ( int i = 0; i < number_of_scaling_factors ; i++ ) {
		
		const int scaled_width = TJSCALED ( image_width, scaling_factor[i] );
		const int scaled_height = TJSCALED ( image_height, scaling_factor[i] );
		if ( scaled_width <= width && scaled_height <= height ) {
			width = scaled_width;
			height = scaled_height;
			break;
		}
	}
}


void BEJPEG::read_header ( void )
{

	int image_width = 0;
	int image_height = 0;

	tjhandle jpeg_decompressor = tjInitDecompress();
	if ( NULL != jpeg_decompressor ) {

		if ( data.size() > 0) {

			int error = tjDecompressHeader3 ( jpeg_decompressor, &data[0], data.size(), &image_width, &image_height, &chrominance_subsampling, &pixel_format );
			tjDestroy(jpeg_decompressor); // error =

			if ( 0 == error ) {
				adjust_dimensions ( image_width, image_height );
			} else {
				throw BEPlugin_Exception ( kJPEGReadHeaderError, tjGetErrorStr() );
			}

		} else {
			throw BEPlugin_Exception ( kErrorParameterMissing );
		}

	} else {
		throw BEPlugin_Exception ( kJPEGInitDecompressorError, tjGetErrorStr() );
	}

}


void BEJPEG::decompress ( void )
{
	read_header();

	tjhandle jpeg_decompressor = tjInitDecompress();
	if ( NULL != jpeg_decompressor ) {

		std::vector<unsigned char> decompressed_image ( width * height * tjPixelSize[pixel_format] );

		if ( data.size() > 0) {
			
			int error = tjDecompress2 ( jpeg_decompressor, &data[0], data.size(), &decompressed_image[0], width, 0, height, pixel_format, 0 );
			tjDestroy ( jpeg_decompressor ); // error =

			if ( 0 == error ) {
				data = decompressed_image;
			} else {
				throw BEPlugin_Exception ( kJPEGDecompressionError, tjGetErrorStr() );
			}

		} else {
			throw BEPlugin_Exception ( kErrorParameterMissing );
		}

	} else {
		throw BEPlugin_Exception ( kJPEGInitDecompressorError, tjGetErrorStr() );
	}

}


void BEJPEG::compress ( void )
{
	tjhandle jpeg_compressor = tjInitCompress();
	if ( NULL != jpeg_compressor ) {

		unsigned long image_size = width * height * tjPixelSize[pixel_format];
		std::vector<unsigned char> image ( image_size );
		unsigned char * compressed_image = &image[0];

		int error = tjCompress2 ( jpeg_compressor, &data[0], width, 0, height, pixel_format, &compressed_image, &image_size, chrominance_subsampling, compression_level, 0 );
		tjDestroy ( jpeg_compressor ); // error =

		if ( 0 == error ) {
			image.resize ( image_size );
			data = image;
		} else {
			throw BEPlugin_Exception ( kJPEGCompressionError, tjGetErrorStr() );
		}

	} else {
		throw BEPlugin_Exception ( kJPEGInitCcompressorError, tjGetErrorStr() );
	}

}

