/*
 BEJSON.cpp
 BaseElements Plug-In
 
 Copyright 2013-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEJSON.h"
#include "BECppUtilities.h"

#include <math.h>


BEJSON::BEJSON ( ) {
	document = NULL;
}


BEJSON::BEJSON ( const std::string& json ) {

	json_error_t json_error;
	document = json_loads ( json.c_str(), 0, &json_error );
	if ( !document ) {
		throw BEJSON_Exception ( json_error );
	}

}


BEJSON::~BEJSON ( ) {
	
	if ( document ) {
		json_decref ( document );
	}

}


void BEJSON::json_path_query ( const std::string& json_path, fmx::Data& results ) {
	
	json_t * node = json_path_get ( document, json_path.c_str() );
		
	if ( node ) {

		std::string string_result;
			
		switch ( json_typeof ( node ) ) {
				
			case JSON_OBJECT:
			case JSON_ARRAY:
				string_result = (char *)json_dumps ( node, 0 );
				SetResult ( string_result, results );
				break;
				
			case JSON_STRING:
				string_result = (char *)json_string_value ( node );
				SetResult ( string_result, results );
				break;
				
			case JSON_INTEGER:
			case JSON_REAL:
				string_result = (char *)json_number_value_as_string ( node );
				SetResult ( string_result, results );
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
			
} // json_path_query


std::string BEJSON::encode ( const std::string& key, const fmx::Data& value, const std::string& type ) {
	
	json_t * json_value;
	bool is_real = false;
	
	int data_type = value.GetNativeType ( );
	
	if ( type == "null" ) {
		
		json_value = json_null ( );

	} else if ( data_type == fmx::Data::kDTBoolean ) {
		
		if ( value.GetAsBoolean ( ) == true ) {
			json_value = json_true ( );
		} else {
			json_value = json_false ( );
		}

	} else if ( data_type == fmx::Data::kDTNumber || data_type == fmx::Data::kDTDate || data_type == fmx::Data::kDTTime || data_type == fmx::Data::kDTTimeStamp ) {
		
		fmx::FixPtUniquePtr number;
		number->AssignFixPt ( value.GetAsNumber() );

		// integer or real value ?
		fmx::TextUniquePtr fmx_text;
		fmx_text->SetText ( value.GetAsText( ) );
		std::string json_text = TextAsNumberString ( *fmx_text );

		fmx::TextUniquePtr decimal_point;
		decimal_point->Assign ( "." );
		
		bool integer = fmx_text->Find ( *decimal_point, 0 ) == fmx::Text::kSize_End;
		
		if ( integer ) {
			if ( json_text.empty() ) {
				json_text = "0";
			}
			json_value = json_integer_with_string ( (json_int_t)number->AsFloat ( ), json_text.c_str() ); // AsLong returns an int (which overflows)
		} else {

			// put it in as string and rip out the unwanted quotes below
			json_value = json_string ( json_text.c_str() );
			is_real = true;
		}
		
	} else if ( data_type == fmx::Data::kDTText ) {
		
		fmx::TextUniquePtr fmx_text;
		fmx_text->SetText ( value.GetAsText( ) );
		std::string json_text = TextAsUTF8String ( *fmx_text );
		json_value = json_string ( json_text.c_str() );

	} else {
		
		// fmx::Data::kDTBinary
		// fmx::Data::kDTInvalid
		
		throw BEJSON_Exception ( kBE_JSON_InvalidDataTypeError );

	}
	
	json_t * json = json_object ( );
	int result = json_object_set ( json, key.c_str(), json_value );
	if ( result ) {
		throw BEJSON_Exception ( result );
	}
	
	size_t flags = JSON_COMPACT;
	char * text = json_dumps ( json, flags );
	std::string out ( text );
	be_free ( text );
	
	// workaround for rounding/precision errors in reals
	if ( is_real ) {
		size_t found = out.find ( "\":\"" );
		out.erase ( found + 2, 1 );
		out.erase ( out.size() - 1 );
	}

	return out;
	
} // encode



