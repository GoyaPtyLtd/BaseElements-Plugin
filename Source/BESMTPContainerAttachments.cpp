/*
 BESMTPContainerAttachments.cpp
 BaseElements Plug-In
 
 Copyright 2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESMTPContainerAttachments.h"

#include "BEPluginGlobalDefines.h"
#include "BEFileSystem.h"
#include "BEPluginUtilities.h"



BESMTPContainerAttachments::~BESMTPContainerAttachments()
{
	clear();
}


void BESMTPContainerAttachments::add ( std::string file_name, std::vector<char>& data )
{
	const fmx::ExprEnvAutoPtr environment;
	FMX_SetToCurrentEnv ( &(*environment) );
	boost::filesystem::path path = GetFileMakerTemporaryDirectory ( *environment ) + BUNDLE_STRINGS_ID + "/SMTP_Attachments/" + file_name;
	
	g_last_error = write_to_file ( path, data );
	
	if ( g_last_error == kNoError ) {
		file_paths.append ( path );
	}
	
}


void BESMTPContainerAttachments::clear ( void )
{
	for ( auto const& delete_this : file_paths.get_values() ) {
		
		try {
			boost::filesystem::remove ( delete_this );
		} catch ( boost::filesystem::filesystem_error& e ) {
			g_last_error = e.code().value();
		}
		
	}
	
	file_paths.clear();
	
}


