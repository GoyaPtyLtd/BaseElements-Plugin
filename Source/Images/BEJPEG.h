/*
 BEJPEG.h
 BaseElements Plug-In

 Copyright 2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#ifndef __BaseElements__BEJPEG__
#define __BaseElements__BEJPEG__


#include "BEImage.h"

#include <libturbojpeg/turbojpeg.h>


class BEJPEG : public BEImage {


public:

	BEJPEG ( );
	explicit BEJPEG ( const std::vector<unsigned char>& image_data, const int image_width = kImageSizeUnknown, const int image_height = kImageSizeUnknown ) : BEImage ( image_data, image_width, image_height ) { BEJPEG (); }


	void set_compression_level ( const int level );
	void recompress ( void  );

	virtual const std::string get_type ( void );

protected:

	void compress ( void );
	const int adjust_dimension ( const int dimension, const int default_dimension, const int other_dimension, const int default_other_dimension );
	void read_header ( void );
	void decompress ( void );

	int pixel_format;
	int compression_level;
	int chrominance_subsampling;

};


#endif /* defined(__BaseElements__BEJPEG__) */
