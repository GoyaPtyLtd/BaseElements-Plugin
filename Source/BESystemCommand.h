/*
 BESystemCommand.h
 BaseElements Plug-In
 
 Copyright 2011-2023 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BESHELL_H)
#define BESHELL_H


#include "BEPluginUtilities.h"
#include <errno.h>
#include "BEPluginException.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomma"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma GCC diagnostic pop

#include <Poco/ActiveMethod.h>



class BESystemCommand
{
	
	public:
	
		struct be_system_command {
			std::string command_text;
			bool execute_using_shell;
		};
		
		
		BESystemCommand():execute(this, &BESystemCommand::execute_implementation) {}

		Poco::ActiveMethod<std::string, be_system_command, BESystemCommand> execute;


	protected:
	
		std::string execute_implementation ( const be_system_command& shell_command );
	//	, const bool execute_using_shell
    
};


#endif // BESHELL_H
