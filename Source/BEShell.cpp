/*
 BEShell.cpp
 BaseElements Plug-In
 
 Copyright 2011-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEPluginGlobalDefines.h"
#include "BEShell.h"


#include <errno.h>


#include "boost/date_time/posix_time/posix_time.hpp"


#if defined(FMX_WIN_TARGET)

	#define POPEN _popen
	#define PCLOSE _pclose

#endif

#if defined(FMX_MAC_TARGET)

	#define POPEN popen
	#define PCLOSE pclose

#endif


using namespace std;


short ExecuteShellCommand ( const string command, string& result )
{
	
	short error = kNoError;
	
	FILE * command_result = POPEN ( command.c_str(), "r" );
	
	if ( command_result ) {
		
		char reply[PATH_MAX];
		
		while ( fgets ( reply, PATH_MAX, command_result ) != NULL ) {
			result.append ( reply );
		}
		
		error = PCLOSE ( command_result );
		
		// both PCLOSE and execl set the error to -1 when setting errno
		if ( error == -1 ) {
			error = errno;
		}
		
		
	} else {
		error = errno;
	}
	
	return error;
	
} // ExecuteShellCommand



#if defined(FMX_MAC_TARGET)



short ExecuteSystemCommand ( const string command, string& result, const long command_timeout )
{

	short error = kNoError;
	
	FILE * command_result = popen ( command.c_str(), "r" );
	
	if ( command_result ) {
		
		char reply[PATH_MAX];

		struct timeval timeout;

		switch ( command_timeout ) {
				
			case kBE_Never:
			case 0:
				timeout.tv_sec = 0;
				timeout.tv_usec = 0;
				break;
				
			default:
				timeout.tv_sec = command_timeout / 1000;
				timeout.tv_usec = (command_timeout % 1000) * 1000;
				
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
				select_response = select ( fileno ( command_result ) + 1, &readfd, NULL, NULL, &timeout );
			} else {
				select_response = select ( fileno ( command_result ) + 1, &readfd, NULL, NULL, NULL );
			}

			if ( select_response < 0 ) {
				return errno;
			} else if ( select_response == 0 ) {
				return kCommandTimeout; // timeout
			} else {
				
				if ( FD_ISSET ( fileno ( command_result ), &readfd ) ) {
					
					if ( fgets ( reply, sizeof ( reply ), command_result ) != NULL ) {
						result.append ( reply );
					} else {
						// nothing returned by the command
						break;
					}
				} else {
//					error = errno;
					break;
				}
			
			} // if ( select_response < 0 )
		
		} // while
	
		short close_error = pclose ( command_result );
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
		
	return error;
	
} // ExecuteSystemCommand

#endif



// see http://msdn.microsoft.com/en-us/library/ms682499(VS.85).aspx

#if defined(FMX_WIN_TARGET)

#include <tchar.h>

#include "BEWinFunctions.h"


short ExecuteSystemCommand ( const string command, string& result, const long command_timeout )
{

	short error = kNoError;

	// Set the bInheritHandle flag so pipe handles are inherited.
	
	SECURITY_ATTRIBUTES security_attributes;
	security_attributes.nLength = sizeof ( SECURITY_ATTRIBUTES );
	security_attributes.bInheritHandle = TRUE;
	security_attributes.lpSecurityDescriptor = NULL;
	
	// Create pipes for the child process's STDIN & STDOUT.
	
	HANDLE command_result = NULL;
	HANDLE command_input = NULL;

	if ( ! CreatePipe ( &command_result, &command_input, &security_attributes, 0 ) ) {
		return GetLastErrorAsFMX();
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	
	if ( ! SetHandleInformation ( command_result, HANDLE_FLAG_INHERIT, 0 ) ) {
		return GetLastErrorAsFMX();
	}

	// Create the child process.
	
	PROCESS_INFORMATION process_information;
	ZeroMemory ( &process_information, sizeof ( PROCESS_INFORMATION ) );
	
	// Specify the STDIN and STDOUT handles for redirection.
	
	STARTUPINFO startup_information;
	ZeroMemory ( &startup_information, sizeof ( STARTUPINFO ) );
	startup_information.cb = sizeof ( STARTUPINFO );
	startup_information.hStdError = command_input;
	startup_information.hStdOutput = command_input;
	startup_information.hStdInput = command_result;
	startup_information.dwFlags |= STARTF_USESTDHANDLES;
	
	// Create the child process.
    
	wstring command_string = utf8toutf16 ( command );
	TCHAR *command_line = (wchar_t *)command_string.c_str();
	DWORD creation_flags = NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW;

	BOOL success = CreateProcess ( NULL, command_line, NULL, NULL, TRUE, creation_flags, NULL, NULL, &startup_information, &process_information );
	
	boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time(); 

	if ( success != 0 ) {
		// Close handles to the child process and its primary thread.

		DWORD timeout = command_timeout == kBE_Never ? INFINITE : command_timeout;
		int wait_result = WaitForSingleObject ( process_information.hProcess, timeout );

		CloseHandle ( process_information.hProcess );
		CloseHandle ( process_information.hThread );
		CloseHandle ( command_input );

		switch ( wait_result ) {

			case WAIT_TIMEOUT:
				error = kCommandTimeout;
//				break;

//			case WAIT_FAILED:
//			case WAIT_ABANDONED:
//			case WAIT_OBJECT_0:
//				break;

			default:

				// Read output from the child process's pipe for STDOUT and Stop when there is no more data.
				
				DWORD number_read;
				CHAR reply [ PATH_MAX + 1 ];
				BOOL did_read = FALSE;
				DWORD read_error = kNoError;

				// only wait for a result when we do not timeout straight away

				if ( timeout != kBE_Immediate ) {

					do {
					
						did_read = ReadFile ( command_result, reply, PATH_MAX, &number_read, NULL );

						if ( did_read && (number_read > 0) ) {
							reply [ number_read ] = '\0';
							result.append ( reply );
						} else {
							read_error = GetLastError ( );
						}

						if ( (command_timeout > kBE_Never) && ( (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds() > command_timeout) ) {
							error = kCommandTimeout;
							break ;
						}
										
					} while ( did_read || read_error == ERROR_IO_PENDING );

//					error = read_error;
				}

				break;
		}

	} else {
		error = GetLastErrorAsFMX ( );
	}

	CloseHandle ( command_result );

	return error;
	
} // ExecuteSystemCommand


#endif

