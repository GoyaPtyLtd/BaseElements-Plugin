//
//  BEJSON.cpp
//  BaseElements
//
//  Created by Mark Banks on 19/02/13.
//  Copyright (c) 2013 Goya. All rights reserved.
//

#include "BEJSON.h"


BEJSON::BEJSON ( StringAutoPtr json ) {

	json_error_t json_error;
	document = json_loadb ( json->c_str(), json->size(), 0, &json_error );
	if ( !document ) {
		throw BEJSON_Exception ( kBE_JSON_LoadDocumentError );
	}

}


BEJSON::~BEJSON ( ) {
	
	json_decref ( document );

}


void BEJSON::json_path_query ( const StringAutoPtr json_path, fmx::Data& results ) {
	
	json_t * node = json_path_get ( document, json_path->c_str() );
		
	if ( node ) {

		StringAutoPtr string_result ( new string );
			
		switch ( json_typeof ( node ) ) {
				
			case JSON_OBJECT:
			case JSON_ARRAY:
				*string_result = (char *)json_dumps ( node, 0 );
				SetResult ( string_result, results );
				break;
				
			case JSON_STRING:
				*string_result = (char *)json_string_value ( node );
				SetResult ( string_result, results );
				break;
				
			case JSON_INTEGER:
				SetResult ( json_integer_value ( node ), results );
				break;
				
			case JSON_REAL:
				SetResultAsDouble ( json_real_value ( node ), results );
				break;
				
			case JSON_TRUE:
				SetResult ( true, results );
				break;
				
			case JSON_FALSE:
				SetResult ( false, results );
				break;
				
			case JSON_NULL:
			default:
				break;
		}
		
	} else {
		throw BEJSON_Exception ( kBE_JSON_JSONPathDoesNotExistError );
	}
			
}


const int BEJSON::last_error()
{
/*
	xmlErrorPtr xml_error = xmlGetLastError();
	
	// don't crash if there's no xml_error
	int error;
	if ( xml_error ) {
		error = xml_error->code;
	} else {
		error = kErrorUnknown;
	}
	
	return error;
*/
	return 0;

}

