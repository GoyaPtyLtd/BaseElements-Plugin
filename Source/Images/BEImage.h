/*
 BEImage.h
 BaseElements Plug-In

 Copyright 2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#ifndef __BaseElements__BEImage__
#define __BaseElements__BEImage__


#include <string>
#include <vector>


enum {
	kImageSizeUnknown = -1
};


class BEImage {


public:

	BEImage ( ) {};
	BEImage ( const std::vector<unsigned char>& image_data, const int image_width = kImageSizeUnknown, const int image_height = kImageSizeUnknown );

	virtual ~BEImage() {};


	const size_t size ( void ) { return data.size(); }

	virtual const std::string get_type ( void ) { return type; }
	const int get_width ( void ) { return width; }
	const int get_height ( void ) { return height; }
	std::vector<unsigned char> get_data ( void ) const { return data; }

	void set_type ( const std::string new_type ) { type = new_type; }
	void set_width ( const int new_width ) { width = new_width; }
	void set_height ( const int new_height ) { height = new_height; }

protected:

	std::vector<unsigned char> data;
	std::string type;
	int width;
	int height;

};


#endif /* defined(__BaseElements__BEImage__) */
