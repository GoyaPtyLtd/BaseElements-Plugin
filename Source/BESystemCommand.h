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

#include <Poco/ActiveMethod.h>


class SystemCommand
{
	public:
	
		SystemCommand(): execute(this, &SystemCommand::execute_implementation) {}
	
		Poco::ActiveMethod<std::string, std::string, SystemCommand> execute;
	
	protected:
	
	std::string execute_implementation ( const std::string& shell_command );// {
	
};


#endif // BESHELL_H
