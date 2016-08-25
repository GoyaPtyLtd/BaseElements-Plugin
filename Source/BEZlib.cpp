/*
 BEZlib.cpp
 BaseElements Plug-In
 
 Copyright 2011-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


// this file based on the minizip examples


#include "BEZlib.h"
#include "BE_Boost_FileSystem_Additions.hpp"
#include "BEPluginException.h"

#include <sys/stat.h>
#include <iostream>
#include <fstream>

#include "zlib.h"

#include <Poco/Delegate.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Zip/Compress.h>
#include <Poco/Zip/Decompress.h>


#define WRITEBUFFERSIZE 8192


class Zip_Error {

public:

	void zip_error ( const void* /* pSender */, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info ) {
		throw BEPlugin_Exception ( kZipError, info.second );
	}

};


#pragma mark -
#pragma mark UnZip Functions
#pragma mark -


const long UnZip ( const std::string& archive, const std::string& output_directory )
{
	long error = kNoError;
	
	Poco::File archive_path = archive;

	if ( archive_path.exists() ) {
		
		std::ifstream out ( archive.c_str(), std::ios::binary );
		poco_assert ( out );

		Poco::Path decompress_here ( output_directory );
		if ( output_directory.empty() ) {
			Poco::Path where = archive;
			decompress_here = where.parent();
		}

		Poco::Zip::Decompress to_decompress ( out, decompress_here );
		to_decompress.EError += Poco::Delegate<Zip_Error, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(NULL, &Zip_Error::zip_error);
		to_decompress.decompressAllFiles();
		to_decompress.EError += Poco::Delegate<Zip_Error, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(NULL, &Zip_Error::zip_error);

	} else {
		error = kNoSuchFileOrDirectoryError;
	}

	return error;
	
} // UnZip


#pragma mark -
#pragma mark Zip Functions
#pragma mark -


const long Zip ( const BEValueList<std::string> * filenames, const std::string& archive )
{

    long error = 0;
	
	const Poco::File file = filenames->first();
	if ( file.exists() ) {
		
		Poco::Path archive_path = archive;
		if ( archive.empty() ) {
			archive_path = file.path() + ".zip";
		}

		std::ofstream out ( archive_path.toString().c_str(), std::ios::binary );
		Poco::Zip::Compress to_compress ( out, true );

		for ( size_t i = 0 ; i < filenames->size() ; i++ ) {
				
			Poco::Path archive_this ( filenames->at ( i ) );
			Poco::File file_to_archive ( archive_this );

			if ( file_to_archive.exists() ) {

				if ( file_to_archive.isDirectory() ) {
					to_compress.addRecursive ( archive_this, Poco::Zip::ZipCommon::CL_MAXIMUM, false, archive_this.getFileName() );
				} else {
					to_compress.addFile ( archive_this, archive_this.getFileName() );
				}

			} else {
				error = kNoSuchFileOrDirectoryError;
				break;
			}
				
		}

		to_compress.close();

	} else {
		error = kNoSuchFileOrDirectoryError;
	}
	
    return error;
	
} // Zip


#pragma mark -
#pragma mark [Un]Gzip
#pragma mark -


const std::vector<char> CompressContainerStream ( const std::vector<char> data )
{
	size_t size_required = compressBound ( data.size() );

	z_stream stream;
	stream.next_in = (unsigned char *)&data[0];
	stream.avail_in = (unsigned int)data.size();
	stream.total_out = 0;
	stream.opaque = Z_NULL; // updated to use default allocation functions.
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;

	std::vector<char> compressed;
	
	int status = deflateInit2 ( &stream, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY );
	if ( status == Z_OK ) {

		unsigned char * output_buffer = new unsigned char [ size_required ];
		
		stream.next_out = output_buffer;
		stream.avail_out = (unsigned int)size_required;

		status = deflate ( &stream, Z_FINISH );
		if ( status == Z_STREAM_END || status == Z_OK ) {

			status = deflateEnd ( &stream );
			if ( status == Z_OK ) {
				compressed.assign ( output_buffer, output_buffer + stream.total_out );
			}
		
		}
		
	}
	
	g_last_error = status;
	
	return compressed;
	
} // CompressContainerStream


const std::vector<char> UncompressContainerStream ( const std::vector<char> data )
{
	z_stream stream;
	stream.next_in = (unsigned char *)&data[0];
	stream.avail_in = (unsigned int)data.size();
	stream.total_out = 0;
	stream.opaque = Z_NULL; // updated to use default allocation functions.
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;

	std::vector<char> decompressed;
 
	int status = inflateInit2 ( &stream, 15 + 32 );
	if ( status == Z_OK ) {

		while ( status == Z_OK && stream.avail_in > 0 ) {
			
			unsigned char output_buffer [ WRITEBUFFERSIZE ];
			stream.next_out = output_buffer;
			stream.avail_out = WRITEBUFFERSIZE;
			
			status = inflate ( &stream, Z_SYNC_FLUSH );
			if ( status == Z_OK || status == Z_STREAM_END ) {
				decompressed.insert ( decompressed.end(), output_buffer, output_buffer + WRITEBUFFERSIZE - stream.avail_out );
			}
		}
		
		int close_status = inflateEnd ( &stream );
		
		if ( status == Z_OK || status == Z_STREAM_END ) {
			status = close_status;
		} else {
			decompressed.clear();
		}
		
	}
 
	g_last_error = status;

	return decompressed;
	
} // UncompressContainerStream

