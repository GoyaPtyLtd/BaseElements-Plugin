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


const std::string BEFMS::list_files ( const bool show_statistics )
{
	return run_command ( "list files", show_statistics );
}


const std::string BEFMS::pause_files ( const std::string files )
{
	return run_command ( "pause " + files );
}


const std::string BEFMS::resume_files ( const std::string files )
{
	return run_command ( "resume " + files );
}


const std::string BEFMS::close_files ( const std::string files )
{
	return run_command ( "close " + files );
}


const std::string BEFMS::open_files ( const std::string files )
{
	return run_command ( "open " + files );
}


const std::string BEFMS::remove_files ( const std::string files )
{
	return run_command ( "remove " + files );
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


const std::string BEFMS::list_clients ( const bool show_statistics )
{
	return run_command ( "list clients", show_statistics );
}


#pragma mark -
#pragma mark protected
#pragma mark -


const std::string BEFMS::run_command ( const std::string command, const bool show_statistics )
{
	const std::string s_flag = show_statistics ? " -s" : "";
	const std::string fms = std::string ( FMSADMIN_PATH ) + "fmsadmin -u " + username + " -p " + password + s_flag + " -y " + command;

	std::string result;
	short return_code = ExecuteSystemCommand ( fms, result );
	if ( kNoError != return_code && result.empty() ) {
		throw BEPlugin_Exception ( return_code );
	}

	return result;
}


