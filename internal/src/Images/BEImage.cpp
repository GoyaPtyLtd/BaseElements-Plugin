/*
 BEImage.cpp
 BaseElements Plug-In

 Copyright 2015-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEImage.h"


BEImage::BEImage ( const std::vector<unsigned char>& image_data, const int image_width, const int image_height )
{
	data.assign ( image_data.begin(), image_data.end() );
	width = image_width;
	height = image_height;
}

