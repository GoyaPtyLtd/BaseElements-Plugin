/*
 BEShell.h
 BaseElements Plug-In
 
 Copyright 2011-2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BESHELL_H)
#define BESHELL_H


#include "BEPluginUtilities.h"


short ExecuteShellCommand ( const string command, string& result ); // depreciated in v2.0
short ExecuteSystemCommand ( const string command, string& result, const long command_timeout );


#endif // BESHELL_H
