/*
 BEJavaScript.cpp
 BaseElements Plug-In
 
 Copyright 2014-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEJavaScript.h"
#include "BEPluginUtilities.h"

#include "duktape/duktape.h"


#pragma mark -
#pragma mark Prototypes
#pragma mark -

static duk_ret_t BE_ExecuteScript ( duk_context *context );
static duk_ret_t BE_Evaluate_FileMaker_Calculation ( duk_context *context );


#pragma mark -
#pragma mark Functions
#pragma mark -

static duk_ret_t evaluate_raw ( duk_context *context ) {
	
	duk_eval ( context );
	return 1;

}


static duk_ret_t tostring_raw ( duk_context *context ) {
	
	duk_to_string ( context, -1 );
	return 1;

}


std::string Evaluate_JavaScript ( const std::string& javascript )
{
	duk_context *context = duk_create_heap_default();

	duk_push_global_object ( context );
	duk_push_c_function ( context, BE_ExecuteScript, 3 );
	duk_put_prop_string ( context, -2, "BE_ExecuteScript" );
	
	duk_push_global_object ( context );
	duk_push_c_function ( context, BE_Evaluate_FileMaker_Calculation, 1 );
	duk_put_prop_string ( context, -2, "BE_Evaluate_FileMaker_Calculation" );
	
	// the scipt itself must be pushed after the c++ functions
	
	duk_push_string ( context, javascript.c_str() );
	duk_safe_call ( context, evaluate_raw, 1, 1 );
	duk_safe_call ( context, tostring_raw, 1, 1 );
	
	std::string out ( duk_get_string ( context, -1 ) );
	
	duk_pop ( context );
	
	duk_destroy_heap ( context );
	
	return out;
}


#pragma mark -
#pragma mark Calling back into the plugin
#pragma mark -

static duk_ret_t BE_ExecuteScript ( duk_context *context )
{
	fmx::errcode error = kNoError;
	
	std::string script = duk_require_string ( context, 0 );
	std::string file_name = duk_require_string ( context, 1 );
	std::string parameter = duk_require_string ( context, 2 );
	
	fmx::TextUniquePtr script_name;
	script_name->Assign ( script.c_str() );
	
	fmx::TextUniquePtr database;
	database->Assign ( file_name.c_str() );
	
	fmx::DataUniquePtr script_parameter;
	fmx::TextUniquePtr parameter_data;
	parameter_data->Assign ( parameter.c_str() );
	fmx::LocaleUniquePtr default_locale;

	script_parameter->SetAsText ( *parameter_data, *default_locale );
	
	const fmx::ExprEnvUniquePtr environment;

	FMX_SetToCurrentEnv ( &(*environment) );
	
	error = ExecuteScript ( *script_name, *database, *script_parameter, *environment );
	
	duk_push_int ( context, error );

	return 1;
}



static duk_ret_t BE_Evaluate_FileMaker_Calculation ( duk_context *context )
{
	fmx::errcode error = kNoError;

	std::string calculation = duk_require_string ( context, 0 );

	fmx::TextUniquePtr command;
	command->Assign ( calculation.c_str() );

	fmx::DataUniquePtr result;
	const fmx::ExprEnvUniquePtr environment;
	FMX_SetToCurrentEnv ( &(*environment) );

	error = environment->Evaluate ( *command, *result );
	
	fmx::TextUniquePtr reply;
	reply->SetText ( result->GetAsText() );
	
	std::string output = TextAsUTF8String ( *reply );

	duk_push_string ( context, output.c_str() );

	return 1;
}

