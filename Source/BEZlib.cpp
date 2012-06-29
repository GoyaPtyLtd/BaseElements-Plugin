/*
 BEZlib.cpp
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


// this file based on the minizip examples


#include "BEZlib.h"
#include "BEPluginGlobalDefines.h"

#include "BE_Boost_FileSystem_Additions.hpp"

#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"

#include <sys/stat.h>

#include "minizip/zip.h"
#include "minizip/unzip.h"


#if defined( _MSC_VER )

	#include <Windows.h>

	#define PATH_MAX MAX_PATH

	#define USEWIN32IOAPI
	#include "minizip/iowin32.h"

	#include <sys/utime.h>

	#ifndef __USE_FILE_OFFSET64
		#define __USE_FILE_OFFSET64
	#endif
	#ifndef __USE_LARGEFILE64
		#define __USE_LARGEFILE64
	#endif
	#ifndef _LARGEFILE64_SOURCE
		#define _LARGEFILE64_SOURCE
	#endif
	#ifndef _FILE_OFFSET_BIT
		#define _FILE_OFFSET_BIT 64
	#endif

#else

	#include <utime.h>

#endif


#define WRITEBUFFERSIZE 8192

using namespace boost::filesystem;


#pragma mark -
#pragma mark Function Declarations
#pragma mark -

void ChangeFileDate ( path file, tm_unz tmu_date );
int ExtractCurrentFile ( path parent, unzFile uf );

void FileTime ( const path file, zip_fileinfo * zi );
bool IsLargeFile ( const path file );
int AddFileToArchive ( const path filename, zipFile zf, const path base );
int AddToArchive ( const path directory_path, zipFile zf, const path base );


#pragma mark -
#pragma mark UnZip Functions
#pragma mark -


void ChangeFileDate ( path file, tm_unz tmu_date )
{

	struct tm newdate;
	newdate.tm_sec = tmu_date.tm_sec;
	newdate.tm_min = tmu_date.tm_min;
	newdate.tm_hour = tmu_date.tm_hour;
	newdate.tm_mday = tmu_date.tm_mday;
	newdate.tm_mon = tmu_date.tm_mon;

	if ( tmu_date.tm_year > 1900 ) {
		newdate.tm_year = tmu_date.tm_year - 1900;
	} else {
		newdate.tm_year = tmu_date.tm_year ;		
	}
	
	newdate.tm_isdst = -1;

	struct utimbuf ut;
	ut.actime = ut.modtime = mktime ( &newdate );

	utime ( file.string().c_str(), &ut );

} // ChangeFileDate


int UnZip ( const StringAutoPtr archive )
{
	short error = kNoError;
	
	path archive_path = *archive;
	archive_path.make_preferred();

	unzFile uf = unzOpen64 ( archive_path.string().c_str() );

	if ( uf != NULL ) {

	    unz_global_info64 gi;
	    error = unzGetGlobalInfo64 ( uf, &gi );
	    if ( error == UNZ_OK ) {
		
			for ( ulong i = 0; i < gi.number_entry; i++ ) {
			
				error = ExtractCurrentFile ( archive_path.parent_path(), uf );
				if ( error == UNZ_OK ) {
				
					if ( ( i + 1 ) < gi.number_entry ) {
						error = unzGoToNextFile ( uf );
					}

				}
			
				if ( error != UNZ_OK ) {
					break;
				}
			}
		}

		int close_error = unzClose ( uf );
		if ( error == UNZ_OK ) {
			error = close_error;
		}
		
	}
	
	return error;
	
} // UnZip


int ExtractCurrentFile ( path parent, unzFile uf )
{
    int error = UNZ_OK;

    char filename_inzip[PATH_MAX];
    unz_file_info64 file_info;
	
    error = unzGetCurrentFileInfo64 ( uf, &file_info, filename_inzip, sizeof ( filename_inzip ), NULL, 0, NULL, 0 );
	
	path file = filename_inzip;

	file.make_preferred();
	parent.make_preferred();

	if ( error == UNZ_OK ) {

		string macos = "__MACOSX";
		if ( file.string().compare ( 0, macos.length(), macos ) == 0 ) {
			return kNoError;
		}
		
		path to_write = parent / file;
		create_directories ( to_write.parent_path() );

		if ( file.filename().string() == "." ) {
			
			create_directory ( to_write );
			
		} else {
			
			error = unzOpenCurrentFilePassword ( uf, NULL );

			if ( error == UNZ_OK ) {

				char * buffer = new char [ WRITEBUFFERSIZE ];
				
				boost::filesystem::ofstream output_file ( to_write, ios_base::binary );
				output_file.exceptions ( boost::filesystem::ofstream::badbit | boost::filesystem::ofstream::failbit );			
				
				long bytes_read = 0;
				do {
					bytes_read = unzReadCurrentFile ( uf, (void *)buffer, WRITEBUFFERSIZE );
					
					if ( bytes_read > 0 ) {
						output_file.write ( buffer, bytes_read );
					} else if ( bytes_read < 0 ) {
						error = bytes_read;
					}
				} while ( bytes_read > 0 );
								
				output_file.close();
                ChangeFileDate ( to_write, file_info.tmu_date );
				delete [] buffer;

			}
			
			// don't lose the error
			int close_error = unzCloseCurrentFile ( uf );
			if ( error == UNZ_OK ) {
				error = close_error;
			}
		}
	}
	    
	return error;

} // ExtractCurrentFile


#pragma mark -
#pragma mark Zip Functions
#pragma mark -


void FileTime ( const path file, zip_fileinfo * zi )
{
	time_t when = last_write_time ( file );
	struct tm * filedate = localtime ( &when );
	
	tm_zip tmzip = zi->tmz_date;
	
	tmzip.tm_sec  = filedate->tm_sec;
	tmzip.tm_min  = filedate->tm_min;
	tmzip.tm_hour = filedate->tm_hour;
	tmzip.tm_mday = filedate->tm_mday;
	tmzip.tm_mon  = filedate->tm_mon ;
	tmzip.tm_year = filedate->tm_year;
	
#if defined( _MSC_VER )

	FILETIME ftLocal;
    HANDLE hFind;
    WIN32_FIND_DATAA ff32;

	uLong * dt = &(zi->dosDate);

    hFind = FindFirstFileA ( file.string().c_str(), &ff32 );
    if ( hFind != INVALID_HANDLE_VALUE ) {
		FileTimeToLocalFileTime ( &(ff32.ftLastWriteTime), &ftLocal );
        FileTimeToDosDateTime ( &ftLocal, ((LPWORD)dt) + 1, ((LPWORD)dt) + 0 );
		zi->dosDate = *dt;
        FindClose ( hFind );
	}

#endif


} // FileTime


bool IsLargeFile ( const path file )
{
	return file_size ( file ) >= 0xffffffff;
}


int AddFileToArchive ( const path filename, zipFile zf, const path base )
{		
    int error = kNoError;
	
	zip_fileinfo zi;
	FileTime ( filename, &zi );
	
	string relative_path = NaiveUncomplete ( filename, base ).string();
	int compression_level = Z_BEST_COMPRESSION;
	const char* password = NULL;
	unsigned long crcFile = 0;
	bool zip64 = IsLargeFile ( filename );

	error = zipOpenNewFileInZip3_64 ( zf,
									 relative_path.c_str(),
									 &zi,
									 NULL,
									 0,
									 NULL,
									 0,
									 NULL,
									 (compression_level != 0) ? Z_DEFLATED : 0,
									 compression_level,
									 0,
									 -MAX_WBITS, 
									 DEF_MEM_LEVEL, 
									 Z_DEFAULT_STRATEGY,
									 password,
									 crcFile, 
									 zip64
									 );
	
	if ( error == ZIP_OK ) {
		
		char * buffer = new char [ WRITEBUFFERSIZE ];

		try {
			
			boost::filesystem::ifstream input_file ( filename, ios_base::in | ios_base::binary );
			input_file.exceptions ( boost::filesystem::ifstream::badbit );

			streamsize size_read = 0;
							
			do {
				
				input_file.read ( buffer, WRITEBUFFERSIZE );
				size_read = input_file.gcount();

				if ( size_read > 0 ) {
					error = zipWriteInFileInZip ( zf, (void *)buffer, (unsigned int)size_read );
				} else if ( size_read < 0 ) {
					error = ZIP_ERRNO;
				}
					
			} while ( (error == ZIP_OK) && (!input_file.eof()) );//&& (size_read > 0) );
				
			input_file.close();
				
		} catch ( filesystem_error& e ) {
			error = e.code().value();
		}
		
		delete [] buffer;

		// don't loose the error
		int close_error = zipCloseFileInZip ( zf );
		if ( error == ZIP_OK ) {
			error = close_error;
		}
		
	} else {
		error = ZIP_ERRNO;	//	error in opening file for reading
	}
	
	return error;
	
} // AddFileToArchive


int AddToArchive ( const path directory_path, zipFile zf, const path base )
{
    int error = 0;
	
	try {
		
		if ( exists ( directory_path ) ) {
			
			if ( ! is_directory ( directory_path ) ) {
				error = AddFileToArchive ( directory_path, zf, base );
			} else {

				directory_iterator end_itr; // default construction yields past-the-end
				directory_iterator itr ( directory_path );
				
				while ( itr != end_itr ) {
					error = AddToArchive ( itr->path(), zf, base );
					++itr;					
				}
			}
		}
		
	} catch ( filesystem_error& e ) {
		error = e.code().value();
	}
	
	return error;
	
} // AddToArchive


int Zip ( StringAutoPtr filename )
{
    int error = 0;
	
	path file = *filename;

	if ( exists ( file ) ) {

		string archive_name = *filename + ".zip";
		zipFile zf;
		
		bool dont_overwrite = false;
		
#ifdef USEWIN32IOAPI
		zlib_filefunc64_def ffunc;
		fill_win32_filefunc64A(&ffunc);
		zf = zipOpen2_64 ( archive_name.c_str(), dont_overwrite, NULL, &ffunc );
#else
		zf = zipOpen64 ( archive_name.c_str(), dont_overwrite );
#endif
		
		if ( zf == NULL ) {
			error = ZIP_ERRNO;
		} else {
			
			path base = file.parent_path();
			error = AddToArchive ( file, zf, base );
			
		}
		
		// only worry about an error closing the file if there hasn't already been one
		int close_error = zipClose ( zf, NULL );
		if ( error == kNoError ) {
			error = close_error;
		}
		
	} else {
		error = kNoSuchFileOrDirectoryError;
	}

    return error;
	
} // Zip


