/*
 BEFMS.h
 BaseElements Plug-In
 
 Copyright (c) 2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef BEFMS_h
#define BEFMS_h


#include <string>
#include <vector>


class BEFMS {
	
	
public:
	

	BEFMS ( const std::string _username, const std::string _password );

	
	const std::string upload_database ( const std::vector<char> database, const bool replace_existing, const std::string destination );
	const std::string download_database ( const std::vector<char> database, const bool stop );

	const std::string list_files();
	const std::string pause_files ( const std::string files = "" );
	const std::string run_schedule ( const long schedule_number );
	const std::string list_schedules();
	const std::string list_clients();
				
	
protected:
	
	std::string username;
	std::string password;
	
	const std::string run_command ( const std::string command );
	
};


#endif /* BEFMS_h */
