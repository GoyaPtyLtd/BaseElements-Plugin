/*
 BESystemCommand.h
 BaseElements Plug-In
 
 Copyright 2011-2021 Goya. All rights reserved.
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

class SystemCommand
{
	
	public:
	
	struct system_command {
		std::string command_text;
		bool execute_using_shell;
	};
	
	
    SystemCommand():execute(this, &SystemCommand::execute_implementation) {}
    
            Poco::ActiveMethod<std::string, system_command, SystemCommand> execute;


	protected:
	
        std::string execute_implementation ( const system_command& shell_command );
	//	, const bool execute_using_shell
    
};


#endif // BESHELL_H
