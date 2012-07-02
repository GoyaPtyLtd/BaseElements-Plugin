/*
 BEFileMakerPlugin.cpp
 BaseElements Plug-In
 
 Copyright 2010-2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEFileMakerPlugin.h"

#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"

#include <iostream>
#include <vector>


#pragma mark -
#pragma mark Constructors
#pragma mark -

BEFileMakerPlugin::BEFileMakerPlugin ( QuadCharAutoPtr plugin_id )
{
	id = plugin_id;
}


BEFileMakerPlugin::~BEFileMakerPlugin()
{
	// un-register all registered functions
	vector<short>::iterator it = registered_functions.begin();
	while ( it != registered_functions.end() ) {
		ExprEnv::UnRegisterExternalFunction ( *id, *it );
		++it;
	}
}

#pragma mark -
#pragma mark Methods
#pragma mark -

errcode BEFileMakerPlugin::RegisterFunction ( short function_id, ExtPluginType function_pointer, bool server_compatible, int minimum_parameters, int maximum_parameters )
{
	ulong function_flags = ExprEnv::kDisplayInAllDialogs;
	if ( server_compatible ) {
		function_flags = ExprEnv::kMayEvaluateOnServer | function_flags;
	}
	
	int max_params = maximum_parameters ? maximum_parameters : minimum_parameters;
	
	errcode error_result = 0;
	TextAutoPtr name, prototype;
	
	Do_GetString ( function_id, name, true );
	Do_GetString ( function_id, prototype, false );
	
//	QuadCharAutoPtr plugin_id ( PLUGIN_ID );
	
	error_result = ExprEnv::RegisterExternalFunction ( *id, function_id, *name, *prototype, (short)minimum_parameters, (short)max_params, function_flags, function_pointer );
	
	// keep track of registered functions so that they can be automatically unregistered
	
	if ( error_result == kNoError ) {
		registered_functions.push_back ( function_id );	
	}
	
	return error_result;
	
}	//	RegisterFunction


// register functions that take no optional parameters

errcode BEFileMakerPlugin::RegisterFunction ( short function_id, ExtPluginType function_pointer, int minimum_parameters )
{
	return RegisterFunction ( function_id, function_pointer, true, minimum_parameters, minimum_parameters );
}

