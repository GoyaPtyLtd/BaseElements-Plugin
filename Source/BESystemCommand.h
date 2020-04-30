/*
 BESystemCommand.h
 BaseElements Plug-In
 
 Copyright 2011-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BESHELL_H)
#define BESHELL_H


#include "BEPluginUtilities.h"
#include <errno.h>
#include "BEPluginException.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <Poco/ActiveMethod.h>

class SystemCommand
{
	public:
	
    SystemCommand():execute(this, &SystemCommand::execute_implementation) {}
    
            Poco::ActiveMethod<std::string, std::string, SystemCommand> execute;
    
    std::string run_with_popen( const std::string& shell_command, const long command_timeout = kBE_Never  ) ;
    
	protected:
	
        std::string execute_implementation ( const std::string& shell_command ); //
    
};


#endif // BESHELL_H
