/*
 BEFileSystem.cpp
 BaseElements Plug-In
 
 Copyright 2011-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEFileSystem.h"
#include "BEPluginException.h"


using namespace std;
using namespace boost::filesystem;


bool recursive_directory_copy ( const path & from, const path & to  ) {
	
//	if ( !exists ( from ) ) return false;
	
	path base = from;

	if ( is_directory ( from ) ) {

		copy_directory ( from, to );

		directory_iterator end_itr; // default construction yields past-the-end
		
		for ( directory_iterator itr ( from ); itr != end_itr; ++itr ) {
			
			path relative = from;
			
			path target = to;
			target /= itr->path().filename();
			
			bool failed = recursive_directory_copy ( itr->path(), target );
			if ( !failed ) {
				return false;
			}

		}
		
	} else {
		
		// note: aliases & symlinks are not handled correctly
		
		copy ( from, to );
	}
	
	return true;
	
} // recursive_directory_copy


BEValueListWideStringAutoPtr list_files_in_directory ( const boost::filesystem::path & directory, const long file_type_wanted = kBE_FileType_ALL, const bool recurse = false )
{

	BEValueListWideStringAutoPtr list_of_files ( new BEValueList<wstring> );
	
	try {
		
		path directory_path = directory;
		bool directory_exists = exists ( directory_path );
		
		if ( directory_exists ) {
			
			directory_iterator end_itr; // default construction yields past-the-end
			directory_iterator itr ( directory_path );
						
			while ( itr != end_itr ) {
				
				bool is_folder = is_directory ( itr->status() );
				
				if ( is_folder && recurse ) {
					list_of_files->append ( *list_files_in_directory ( itr->path(), file_type_wanted, recurse ) );
				}

				if (
					(!is_folder && (file_type_wanted == kBE_FileType_File)) ||
					(is_folder && (file_type_wanted == kBE_FileType_Folder)) ||
					(file_type_wanted == kBE_FileType_ALL)
					) {
					
					list_of_files->append ( itr->path().wstring() );

				}
				
				++itr;
				
			}
			
		} else {
			throw BEPlugin_Exception ( kNoSuchFileOrDirectoryError );
		}
		
	} catch ( filesystem_error& e ) {
		throw BEPlugin_Exception ( e.code().value() );
	}
	
	return list_of_files;

} // list_files_in_directory

