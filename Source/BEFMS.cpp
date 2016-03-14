/*
 BEFMS.cpp
 BaseElements Plug-In
 
 Copyright (c) 2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEFMS.h"

#include "BEPluginException.h"
#include "BEShell.h"

#include <sstream>



#ifdef FMX_MAC_TARGET
	#define FMSADMIN_PATH "/usr/local/bin/"
#else
	#define FMSADMIN_PATH ""
#endif



BEFMS::BEFMS ( const std::string _username, const std::string _password )
{
	username = _username;
	password = _password;
}


const std::string BEFMS::upload_database ( const std::vector<char> database, const bool replace_existing, const std::string destination )
{
	return "not implemented";
}


const std::string BEFMS::download_database ( const std::vector<char> database, const bool stop )
{
	return "not implemented";
}


const std::string BEFMS::list_files()
{
	return run_command ( "list files" );
}


const std::string BEFMS::pause_files ( const std::string files )
{
	return run_command ( "pause files " + files );
}


const std::string BEFMS::run_schedule ( const long schedule_number )
{
	std::stringstream schedule;
	schedule << schedule_number;
	
	return run_command ( "run schedule " + schedule.str() );
}


const std::string BEFMS::list_schedules()
{
	return run_command ( "list schedules" );
}


const std::string BEFMS::list_clients()
{
	return run_command ( "list clients" );
}


#pragma mark -
#pragma mark protected
#pragma mark -


const std::string BEFMS::run_command ( const std::string command )
{
	const std::string fms = std::string ( FMSADMIN_PATH ) + "fmsadmin -u " + username + " -p " + password + " " + command;

	std::string result;
	short return_code = ExecuteSystemCommand ( fms, result );
//	if ( kNoError != return_code ) {
//		throw BEPlugin_Exception ( return_code );
//	}

	return result;
}


