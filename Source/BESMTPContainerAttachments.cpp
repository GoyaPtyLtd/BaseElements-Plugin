/*
 BESMTPContainerAttachments.cpp
 BaseElements Plug-In
 
 Copyright 2016~2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESMTPContainerAttachments.h"

#include "BEPluginGlobalDefines.h"
#include "BEFileMakerPlugin.h"
#include "BEFileSystem.h"
#include "BEPluginUtilities.h"


extern BEFileMakerPlugin * g_be_plugin;


BESMTPContainerAttachments::~BESMTPContainerAttachments()
{
	clear();
}


void BESMTPContainerAttachments::add ( const boost::filesystem::path path, const std::string content_type )
{
	files.push_back ( std::make_pair ( path, content_type ) );
}


void BESMTPContainerAttachments::add ( const std::string file_name, const std::vector<char>& data, const std::string content_type )
{
	boost::filesystem::path path = temporary_attachments_directory() + file_name;
	
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
				boost::filesystem::remove ( path );
			}
		} catch ( boost::filesystem::filesystem_error& e ) {
			g_last_error = e.code().value();
		}
		
	}
	
	files.clear();
	
}


// Private Functions


const std::string BESMTPContainerAttachments::temporary_attachments_directory ( void )
{
	std::string temporary_path = "";
	
	 // fms16 gives a per thread directory rather than a global one
	
	if ( g_be_plugin->running_on_server() ) {
		temporary_path = boost::filesystem::temp_directory_path().string();
	} else {
		
		const fmx::ExprEnvUniquePtr environment;
		FMX_SetToCurrentEnv ( &(*environment) );
		temporary_path = GetFileMakerTemporaryDirectory ( *environment );

	}

	temporary_path.append ( BUNDLE_STRINGS_ID );
	temporary_path.append ( "/SMTP_Attachments/" );
	
	return temporary_path;
	
}

