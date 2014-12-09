/*
 BEJavaScript.cpp
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEJavaScript.h"

#include "duktape/duktape.h"


static int evaluate_raw ( duk_context *context ) {
	
	duk_eval ( context );
	return 1;

}


static int tostring_raw ( duk_context *context ) {
	
	duk_to_string ( context, -1 );
	return 1;

}


StringAutoPtr Evaluate_JavaScript ( const StringAutoPtr javascript )
{
	duk_context *context = duk_create_heap_default();

	duk_push_string ( context, javascript->c_str() );
	duk_safe_call ( context, evaluate_raw, 1, 1 );
	duk_safe_call ( context, tostring_raw, 1, 1 );
	
	StringAutoPtr out ( new std::string ( duk_get_string ( context, -1 ) ) );
	
	duk_pop ( context );
	
	duk_destroy_heap ( context );
	
	return out;
}

