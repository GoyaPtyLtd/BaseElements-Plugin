/*
 BEJavaScript.cpp
 BaseElements Plug-In
 
 Copyright 2014-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEJavaScript.h"
#include "BEPluginUtilities.h"

#include "duktape/duktape.h"


#pragma mark -
#pragma mark Prototypes
#pragma mark -

static duk_ret_t BE_ScriptExecute ( duk_context *context );
static duk_ret_t BE_Evaluate_FileMaker_Calculation ( duk_context *context );


#pragma mark -
#pragma mark Functions
#pragma mark -

static duk_ret_t evaluate_raw ( duk_context * context, void * /* udata */ ) {
	
	duk_eval ( context );
	return 1;

}


static duk_ret_t tostring_raw ( duk_context *context, void * /* udata */ ) {
	
	duk_to_string ( context, -1 );
	return 1;

}


std::string Evaluate_JavaScript ( const std::string& javascript )
{
	duk_context * context = duk_create_heap_default();

	duk_push_global_object ( context );
	duk_push_c_function ( context, BE_ScriptExecute, 3 );
	duk_put_prop_string ( context, -2, "BE_ScriptExecute" );
	
	duk_push_global_object ( context );
	duk_push_c_function ( context, BE_Evaluate_FileMaker_Calculation, 1 );
	duk_put_prop_string ( context, -2, "BE_Evaluate_FileMaker_Calculation" );
	
	// the scipt itself must be pushed after the c++ functions
	
	duk_push_string ( context, javascript.c_str() );
	
	duk_safe_call ( context, evaluate_raw, NULL, 1, 1 );
	duk_safe_call ( context, tostring_raw, NULL, 1, 1 );
	
	std::string out ( duk_get_string ( context, -1 ) );
	
	duk_pop ( context );
	
	duk_destroy_heap ( context );
	
	return out;
}


#pragma mark -
#pragma mark Calling back into the plugin
#pragma mark -

static duk_ret_t BE_ScriptExecute ( duk_context *context )
{
	fmx::errcode error = kNoError;
	
	const std::string script = duk_require_string ( context, 0 );
	const std::string file_name = duk_require_string ( context, 1 );
	const std::string parameter = duk_require_string ( context, 2 );
	
	error = ExecuteScript ( script, file_name, parameter, kFMXT_Pause );

	duk_push_int ( context, error );

	return 1;
}



static duk_ret_t BE_Evaluate_FileMaker_Calculation ( duk_context *context )
{
//	fmx::errcode error = kNoError;

	std::string calculation = duk_require_string ( context, 0 );

	fmx::TextUniquePtr command;
	command->Assign ( calculation.c_str(), fmx::Text::kEncoding_UTF8 );

	fmx::DataUniquePtr result;
	const fmx::ExprEnvUniquePtr environment;
	FMX_SetToCurrentEnv ( &(*environment) );

	environment->Evaluate ( *command, *result ); // error =
	
	fmx::TextUniquePtr reply;
	reply->SetText ( result->GetAsText() );
	
	std::string output = TextAsUTF8String ( *reply );

	duk_push_string ( context, output.c_str() );

	return 1;
}

