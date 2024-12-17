/*
 BESMTPContainerAttachments.cpp
 BaseElements Plug-In

 Copyright 2016~2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BESMTPContainerAttachments.h"

#include "BEPluginGlobalDefines.h"
#include "BEFileMakerPlugin.h"
#include "BEFileSystem.h"
#include "BEPluginUtilities.h"


using namespace std;


extern BEFileMakerPlugin * g_be_plugin;


#define SMTP_ATTACHMENTS_DIRECTORY "SMTP_Attachments"


BESMTPContainerAttachments::~BESMTPContainerAttachments()
{
	clear();
}


void BESMTPContainerAttachments::add ( const filesystem::path path, const std::string content_type )
{
	files.push_back ( std::make_pair ( path, content_type ) );
}


void BESMTPContainerAttachments::add ( const std::string file_name, const std::vector<char>& data, const std::string content_type )
{
	filesystem::path path = temporary_attachments_directory() + file_name;

	g_last_error = write_to_file ( path, data );

	if ( g_last_error == kNoError ) {
		files.push_back ( std::make_pair ( path, content_type ) );
	}

}


void BESMTPContainerAttachments::clear ( void )
{
	for ( auto const& delete_this : files ) {

		try {
			auto temp_dir = temporary_attachments_directory();
			const auto path = delete_this.first;
			if ( path.string().compare ( 0, temp_dir.length(), temp_dir ) == 0 ) {
				filesystem::remove ( path );
			}
		} catch ( filesystem::filesystem_error& e ) {
			g_last_error = e.code().value();
		}

	}

	files.clear();

}


// Private Functions


const std::string BESMTPContainerAttachments::temporary_attachments_directory ( void )
{

	auto temporary_path = filesystem::temp_directory_path().string();
	if ( temporary_path.back() != filesystem::path::preferred_separator ) {
        temporary_path += filesystem::path::preferred_separator;
	}

	temporary_path.append ( BUNDLE_STRINGS_ID );
	temporary_path += filesystem::path::preferred_separator;
	temporary_path.append ( SMTP_ATTACHMENTS_DIRECTORY );
	temporary_path += filesystem::path::preferred_separator;

	return temporary_path;

}

