/*
 BEShell.cpp
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEShell.h"
#include "BEPluginGlobalDefines.h"

#include <errno.h>


#if defined(FMX_WIN_TARGET)

	#include "afxdlgs.h"

	#define POPEN _popen
	#define PCLOSE _pclose

#endif

#if defined(FMX_MAC_TARGET)

	#define POPEN popen
	#define PCLOSE pclose

#endif



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
				
	} else {
		error = errno;
	}
		
	// both PCLOSE and execl set the error to -1 when setting errno
	if ( error == -1 ) {
		error = errno;
	}
		
	return error;
	
} // ShellCommand

