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


std::string SystemCommand::execute_implementation ( const std::string& shell_command ) {

	std::string result;
	short error = kNoError;
	
	try {
		
#if defined ( BOOST_WINDOWS )
		auto command_and_arguments = boost::program_options::split_winmain ( shell_command );
#else
		auto command_and_arguments = boost::program_options::split_unix ( shell_command );
#endif
        
		if ( command_and_arguments.size() > 0 ) { // otherwise we return the empty string.
			
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

// Note: not implemented on iOS
#if ( FMX_MAC_TARGET || FMX_LINUX_TARGET || FMX_IOS_TARGET )

std::string SystemCommand::run_with_popen ( const std::string& shell_command, const long command_timeout ) {
    
    std::string result ;
    std::string bin_sh ;
    
    short error = kNoError;

    if (shell_command.length() > 0) {

    
        FILE * command_result = popen ( shell_command.c_str(), "r" );

        if ( command_result ) {

            short close_error ;
            char reply[PATH_MAX];

            struct timeval timeout;
            struct timeval hold ;
            
            switch ( command_timeout ) {

                case kBE_Never:
                case 0:
                    hold.tv_sec = 0;
                    hold.tv_usec = 0;
                    break;

                default:
                    hold.tv_sec = command_timeout / 1000;
                    hold.tv_usec = (command_timeout % 1000) * 1000;

            } // switch

            boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();

            fd_set readfd;
            FD_ZERO ( &readfd );

            while ( true ) {

                if ( (command_timeout) > kBE_Never && ( (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds() > command_timeout) ) {
                    error = kCommandTimeout;
                    break ;
                }

                FD_SET ( fileno ( command_result ), &readfd );

                int select_response = 0;

                if ( command_timeout > kBE_Never ) {
                    /* reinitialise timeout in case timeout is changed by select */
                    timeout.tv_sec = hold.tv_sec ;
                    timeout.tv_usec = hold.tv_usec ;
                    select_response = select ( fileno ( command_result ) + 1, &readfd, NULL, NULL, &timeout );
                } else {
                    select_response = select ( fileno ( command_result ) + 1, &readfd, NULL, NULL, NULL );
                }

                if ( select_response < 0 ) {
                    pclose ( command_result ); // https://github.com/GoyaPtyLtd/BaseElements-Plugin/issues/166
                    throw BEPlugin_Exception ( errno );
                } else if ( select_response == 0 ) {
                    pclose ( command_result ); // https://github.com/GoyaPtyLtd/BaseElements-Plugin/issues/166
                    throw BEPlugin_Exception ( kCommandTimeout ); // timeout
                } else {

                    if ( FD_ISSET ( fileno ( command_result ), &readfd ) ) {

                        if ( fgets ( reply, sizeof ( reply ), command_result ) != NULL ) {
                            result.append ( reply );
                        } else {
                            // nothing returned by the command
                            break;
                        }
                    } else {
                        //   error = errno;
                        break;
                    }

                } // if ( select_response < 0 )

            } // while

            close_error = pclose ( command_result );
            if ( error == kNoError ) { // don't overwrite an earlier error

                if ( WIFEXITED ( close_error ) ) {
                    error = WEXITSTATUS ( close_error );
                } else {
                    error = close_error;
                }

            } else if ( error == 13 ) { // report an error on command timeout
                error = kCommandTimeout;
            }

        } else {
            error = errno;
        }
        
        if( error != kNoError ) {
            throw BEPlugin_Exception ( error ) ;
        }
        
    } // if shell_command.length() > 0
    
    return result ;
}

#endif

