/*
 BEFileSystem.cpp
 BaseElements Plug-In
 
 Copyright 2011-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEFileSystem.h"
#include "BEPluginException.h"

#include "boost/filesystem/fstream.hpp"


using namespace std;
using namespace boost::filesystem;


const bool recursive_directory_copy ( const path & from, const path & to  ) {
	
	auto base = from;

	if ( is_directory ( from ) ) {

		copy_directory ( from, to );

		directory_iterator end_it; // default construction yields past-the-end
		
		for ( directory_iterator it ( from ); it != end_it; ++it ) {
			
//			auto relative = from;
			
			path target = to;
			target /= it->path().filename();
			
			auto failed = recursive_directory_copy ( it->path(), target );
			if ( !failed ) {
				return false;
			}

		}
		
	} else {
		
		// note: aliases & symlinks are not handled correctly
		
		copy_file ( from, to );
	}
	
	return true;
	
} // recursive_directory_copy


BEValueListWideStringUniquePtr list_files_in_directory ( const boost::filesystem::path & directory, const long file_type_wanted = kBE_FileType_ALL, const bool recurse = false )
{
	
	BEValueListWideStringUniquePtr list_of_files ( new BEValueList<wstring> );
	
	try {
		
		path directory_path = directory;
		directory_path.make_preferred(); // force the correct path separator for the platform
		
		recursive_directory_iterator end_it; // default construction yields past-the-end
		recursive_directory_iterator it ( directory_path, symlink_option::recurse );
			
		while ( it != end_it ) {
				
			if ( !((recurse == false) && (it.level() > 0)) ) {

				bool is_folder = is_directory ( it->status() );
					
				if (
						(!is_folder && (file_type_wanted == kBE_FileType_File)) ||
						(is_folder && (file_type_wanted == kBE_FileType_Folder)) ||
						(file_type_wanted == kBE_FileType_ALL)
					) {
					list_of_files->append ( it->path().wstring() );
				}
				
			}
				
			++it;

		}
		
	} catch ( filesystem_error& e ) {
		throw BEPlugin_Exception ( e.code().value() );
	}
	
	return list_of_files;
	
} // list_files_in_directory


const fmx::errcode write_to_file ( const path& new_file, const vector<char>& contents, const ios_base::openmode flags )
{
	fmx::errcode error = kNoError;
	
	try {
		
		const path parent_directory = new_file.parent_path ();
		if ( ! exists ( parent_directory ) ) {
			boost::filesystem::create_directories ( parent_directory );
		}
		
		const ios_base::openmode mode = ios_base::out | flags;
		boost::filesystem::ofstream output_file ( new_file, mode );
		output_file.exceptions ( boost::filesystem::ofstream::failbit | boost::filesystem::ofstream::badbit );
		
		if ( !contents.empty() ) {
			output_file.write ( &contents.front(), contents.size() );
		} else {
			output_file.flush();
		}
		
		output_file.close();
		
	} catch ( boost::filesystem::ofstream::failure& /* e */ ) {
		error = errno; // cannot write to the file
	} catch ( boost::filesystem::filesystem_error& e ) {
		error = e.code().value();
	}

	return error;
	
} // write_new_file


