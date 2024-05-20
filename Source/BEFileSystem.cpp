/*
 BEFileSystem.cpp
 BaseElements Plug-In

 Copyright 2011-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEFileSystem.h"
#include "BEPluginException.h"

#include <boost/filesystem/fstream.hpp>
#include <Poco/File.h>
#include <Poco/UnicodeConverter.h>
#include <sys/stat.h>


using namespace std;
using namespace boost::filesystem;


BEValueListWideStringUniquePtr list_files_in_directory ( const boost::filesystem::path& directory, const long file_type_wanted = kBE_FileTypeAll, const bool recurse = false, const bool include_hidden = true )
{

	BEValueListWideStringUniquePtr list_of_files ( new BEValueList<wstring> );

	try {

		path directory_path = directory;
		directory_path.make_preferred(); // force the correct path separator for the platform

		recursive_directory_iterator end_it; // default construction yields past-the-end
		recursive_directory_iterator it ( directory_path, directory_options::follow_directory_symlink );

		while ( it != end_it ) {

			bool is_folder = is_directory ( it->status() );

			if ( is_folder && !recurse ) {
				it.disable_recursion_pending(); // don't recurse into sub directories.
			}


#ifdef FMX_WIN_TARGET

			std::string utf8_path;
			Poco::UnicodeConverter::convert ( it->path().wstring(), utf8_path );
			Poco::File this_path ( utf8_path );

#else

			Poco::File this_path ( it->path().string() );

#endif

			auto visible = !this_path.isHidden();

			if (
					((!is_folder && (file_type_wanted == kBE_FileTypeFile)) ||
					(is_folder && (file_type_wanted == kBE_FileTypeFolder)) ||
					(file_type_wanted == kBE_FileTypeAll))
					&&
					(include_hidden || visible )
				) {
				list_of_files->append ( it->path().wstring() );
			}

			++it;

		}

	} catch ( Poco::FileException& e ) {
		throw BEPlugin_Exception ( e.code() );
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
		if ( output_file ) {

            output_file.exceptions ( output_file.failbit | output_file.badbit );

            if ( !contents.empty() ) {
                output_file.write ( &contents.front(), contents.size() );
            } else {
                output_file.flush();
            }

            output_file.close();

		} else {
            error = errno;
        }

	} catch ( const boost::filesystem::ofstream::failure& /* e */ ) {
		error = errno; // cannot write to the file
	} catch ( const boost::filesystem::filesystem_error& e ) {
		error = e.code().value();
    }

	return error;

} // write_new_file


const uintmax_t file_or_directory_size ( const boost::filesystem::path& path )
{
	uintmax_t size = 0;

	BEValueListWideStringUniquePtr list_of_files ( new BEValueList<wstring> );

	if ( boost::filesystem::is_directory ( path ) ) {
		auto directory_listing ( list_files_in_directory ( path, kBE_FileTypeFile, true ) );
		list_of_files->append ( *directory_listing );
	} else {
		list_of_files->append ( path.wstring() );
	}

	for ( size_t i = 0 ;  i < list_of_files->size() ; ++i ) {

		boost::filesystem::path file = list_of_files->at ( i );

#ifdef FMX_WIN_TARGET
		size += file_size ( file );
#else
		if ( ! boost::filesystem::is_symlink ( file ) ) { // boost follows ze aliases
			size += file_size ( file );
		} else {

			struct stat file_status;

			if ( lstat ( file.c_str(), &file_status ) == 0 ) {
				size += file_status.st_size;
			}

		}
#endif

	}

	return size;

} // file_or_directory_size

