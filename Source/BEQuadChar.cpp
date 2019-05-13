/*
 BEQuadChar.cpp
 BaseElements Plug-In

 Copyright 2015-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEQuadChar.h"
#include "BEPluginUtilities.h"


BEQuadChar::BEQuadChar ( const std::string new_type )
{
	if ( QUAD_CHAR_SIZE != new_type.length() ) {
		throw std::out_of_range ( "Type must be exactly 4 characters long" );
	}

	fmx::QuadCharUniquePtr _type ( new_type[0], new_type[1], new_type[2], new_type[3] );
	*type = *_type;

};


BEQuadChar::BEQuadChar ( const fmx::BinaryData& data, const fmx::int32 which )
{
	data.GetType ( which, *type );
};


fmx::QuadCharUniquePtr BEQuadChar::get_type ( ) const
{
	fmx::QuadCharUniquePtr out ( *type );
	return out;
}


const std::string BEQuadChar::as_string ( ) const
{
	fmx::TextUniquePtr stream_type;
	
	for ( int i = 0 ; i < QUAD_CHAR_SIZE ; i++ ) {
		
		auto character = (*type)[i];
		fmx::uint16 * cc = new fmx::uint16[2];
		cc[0] = character;
		cc[1] = '\0';

		fmx::TextUniquePtr type_character;
		type_character->AssignUnicode ( cc );
		delete[] cc;
		stream_type->AppendText ( *type_character );
		
	}

	return TextAsUTF8String ( *stream_type );
	
};


const bool BEQuadChar::is_file ( ) const
{
	return is_type ( FILE_CONTAINER_TYPE );
}


const bool BEQuadChar::is_zlib ( ) const
{
	return is_type ( COMPRESSED_CONTAINER_TYPE );
}


const bool BEQuadChar::is_image ( ) const
{

	return is_type ( JPEG_CONTAINER_TYPE ) ||
			is_type ( GIF_CONTAINER_TYPE ) ||
			is_type ( EPS_CONTAINER_TYPE ) ||
			is_type ( META_CONTAINER_TYPE ) ||
			is_type ( PNG_CONTAINER_TYPE ) ||
			is_type ( BMP_CONTAINER_TYPE ) ||
			is_type ( PDF_CONTAINER_TYPE );

}


const bool BEQuadChar::is_jpeg ( ) const
{
	return is_type ( JPEG_CONTAINER_TYPE );
}


const bool BEQuadChar::is_fork ( ) const
{
	return is_type ( FORK_CONTAINER_TYPE );
}


const bool BEQuadChar::is_image_attribute ( ) const
{
	return is_type ( DPI_CONTAINER_TYPE ) || is_type ( FILENAME_CONTAINER_TYPE ) || is_type ( SIZE_CONTAINER_TYPE );
}


const bool BEQuadChar::is_type ( const std::string new_type ) const
{
	const fmx::QuadCharUniquePtr file ( new_type[0], new_type[1], new_type[2], new_type[3] );

	return *type == *file;

}
