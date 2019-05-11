/*
 BESystemCommand.cpp
 BaseElements Plug-In

 Copyright 2011-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEPluginGlobalDefines.h"
#include "BESystemCommand.h"

#include <boost/program_options/parsers.hpp>

#include <Poco/Process.h>
#include <Poco/Pipe.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>


std::string SystemCommand::execute_implementation ( const std::string& shell_command ) {

	std::string result;
	short error = kNoError;
	
	try {
		
#if defined ( BOOST_WINDOWS )
		auto command_and_arguments = boost::program_options::split_winmain ( shell_command );
#else
		auto command_and_arguments = boost::program_options::split_unix ( shell_command );
#endif
		
		auto command = command_and_arguments.front();
		
		std::vector<std::string> arguments;
		command_and_arguments.erase ( command_and_arguments.begin() ); // auto it = new first element
		for ( auto& each_argument : command_and_arguments ){
			arguments.push_back ( each_argument );
		}
		
		Poco::Pipe output_pipe;
		Poco::ProcessHandle process = Poco::Process::launch ( command, arguments, 0, &output_pipe, &output_pipe );
		Poco::PipeInputStream istr ( output_pipe );
		error = process.wait();
		
		std::ostringstream output;
		Poco::StreamCopier::copyStream ( istr, output );
		
		result = output.str();
		
	} catch ( Poco::SystemException& e ) {
		error = e.code();
	}
	
	return result;

}
