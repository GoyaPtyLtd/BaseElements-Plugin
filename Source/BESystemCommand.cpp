/*
 BESystemCommand.cpp
 BaseElements Plug-In

 Copyright 2011-2020 Goya. All rights reserved.
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


#if defined ( BOOST_WINDOWS )
	#define SPLIT split_winmain
	#define SHELL "cmd"
	#define SHELL_FLAG "/c"
#else
	#define SPLIT split_unix
	#define SHELL "/bin/sh"
	#define SHELL_FLAG "-c"
#endif


std::string SystemCommand::execute_implementation ( const system_command& shell_command ) {

	short error = kNoError;
	std::string result;

	try {
				
		if ( shell_command.command_text.size() > 0 ) { // otherwise return an empty string
		
			std::vector<std::string> command_and_arguments;
			if ( shell_command.execute_using_shell ) {
				
				command_and_arguments.push_back ( SHELL );
				command_and_arguments.push_back ( SHELL_FLAG );
#ifdef FMX_WIN_TARGET
				auto shell_arguments = boost::program_options::SPLIT ( shell_command.command_text );
				command_and_arguments.insert ( command_and_arguments.end(), shell_arguments.begin(), shell_arguments.end() );
#else
				command_and_arguments.push_back ( shell_command.command_text );
#endif
				
			} else {
				command_and_arguments = boost::program_options::SPLIT ( shell_command.command_text );
			}
			

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

		}
		
	} catch ( Poco::SystemException& e ) {
		error = e.code();
	}
	
	return result;

}

