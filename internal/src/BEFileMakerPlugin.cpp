/*
 BEFileMakerPlugin.cpp
 BaseElements Plug-In
 
 Copyright 2010-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEFileMakerPlugin.h"

#include "BEPluginGlobalDefines.h"
#include "BECppUtilities.h"
#include "BEPluginUtilities.h"

#include <iostream>
#include <vector>


using namespace fmx;


#pragma mark -
#pragma mark Constructors
#pragma mark -

BEFileMakerPlugin::BEFileMakerPlugin ( fmx::QuadChar& id )
{
	*plugin_id = id;
}


BEFileMakerPlugin::~BEFileMakerPlugin()
{
	// un-register all registered functions
	std::vector<short>::iterator it = registered_functions.begin();
	while ( it != registered_functions.end() ) {
		fmx::ExprEnv::UnRegisterExternalFunction ( *plugin_id, *it );
		++it;
	}
}

#pragma mark -
#pragma mark Methods
#pragma mark -


const errcode BEFileMakerPlugin::RegisterHiddenFunction ( short function_id, fmx::ExtPluginType function_pointer, int minimum_parameters, int maximum_parameters )
{
	FMX_UInt32 function_flags = 0x00000000; // not visisble anywhere
	
	return RegisterPluginFunction ( function_flags, function_id, function_pointer, minimum_parameters, maximum_parameters );
	
}	//	RegisterFunction



const errcode BEFileMakerPlugin::RegisterFunction ( short function_id, fmx::ExtPluginType function_pointer, int minimum_parameters, int maximum_parameters )
{
	FMX_UInt32 function_flags = ExprEnv::kDisplayInAllDialogs;
	
//	if ( gFMX_ExternCallPtr->extnVersion >= k160ExtnVersion ) {
//		function_flags |= fmx::ExprEnv::kAllDeviceCompatible;
//	}

	return RegisterPluginFunction ( function_flags, function_id, function_pointer, minimum_parameters, maximum_parameters );
	
}	//	RegisterFunction



const errcode BEFileMakerPlugin::RegisterPluginFunction ( fmx::uint32 function_flags, short function_id, ExtPluginType function_pointer, int minimum_parameters, int maximum_parameters )
{
	int max_params = maximum_parameters ? maximum_parameters : minimum_parameters;
	
	errcode error_result = 0;
	TextUniquePtr name, prototype, description;
	
	Do_GetString ( function_id, name, kFunctionName );
	Do_GetString ( function_id, prototype, kFunctionPrototype );
	
	if ( gFMX_ExternCallPtr->extnVersion >= k150ExtnVersion ) {
		Do_GetString ( function_id, description, kFunctionDescription );
		error_result = fmx::ExprEnv::RegisterExternalFunctionEx ( *plugin_id, function_id, *name, *prototype, *description, (short)minimum_parameters, (short)max_params, function_flags, function_pointer );
	} else {
		error_result = fmx::ExprEnv::RegisterExternalFunction ( *plugin_id, function_id, *name, *prototype, (short)minimum_parameters, (short)max_params, function_flags, function_pointer );
	}
	
	// keep track of registered functions so that they can be automatically unregistered
	
	if ( error_result == kNoError ) {
		registered_functions.push_back ( function_id );	
	}
	
	return error_result;
	
}	//	RegisterFunction


void BEFileMakerPlugin::set_fmx_application ( const FMX_Application fmx_application_value )
{
	fmx_application = fmx_application_value;
}


const bool BEFileMakerPlugin::running_on_server ( ) const
{
	return (fmx_application == kFMXT_Server) || (fmx_application == kFMXT_Web) || (fmx_application == kFMXT_SASE);
}


