/*
 BEJSON.cpp
 BaseElements Plug-In
 
 Copyright 2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEJSON.h"


#include <math.h>


BEJSON::BEJSON ( ) {
	document = NULL;
}


BEJSON::BEJSON ( StringAutoPtr json ) {

	json_error_t json_error;
	document = json_loadb ( json->c_str(), json->size(), 0, &json_error );
	if ( !document ) {
		throw BEJSON_Exception ( json_error );
	}

}


BEJSON::~BEJSON ( ) {
	
	if ( document ) {
		json_decref ( document );
	}

}


void BEJSON::json_path_query ( const StringAutoPtr json_path, fmx::Data& results ) {
	
	json_t * node = json_path_get ( document, json_path->c_str() );
		
	if ( node ) {

		StringAutoPtr string_result ( new std::string );
			
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
			
} // json_path_query


void BEJSON::array_size ( fmx::Data& results ) {
	
	SetResult ( json_array_size ( document ), results );

} // array_size



StringAutoPtr BEJSON::encode ( const StringAutoPtr key, const fmx::Data& value, const StringAutoPtr type ) {
	
	json_t * json_value;
	bool is_real = false;
	
	int data_type = value.GetNativeType ( );
	
	if ( *type == "null" ) {
		
		json_value = json_null ( );

	} else if ( data_type == fmx::Data::kDTBoolean ) {
		
		if ( value.GetAsBoolean ( ) == true ) {
			json_value = json_true ( );
		} else {
			json_value = json_false ( );
		}

	} else if ( data_type == fmx::Data::kDTNumber || data_type == fmx::Data::kDTDate || data_type == fmx::Data::kDTTime || data_type == fmx::Data::kDTTimeStamp ) {
		
		fmx::FixPtAutoPtr number;
		number->AssignFixPt ( value.GetAsNumber() );
		
		// integer or real value ?
		fmx::TextAutoPtr fmx_text;
		fmx_text->SetText ( value.GetAsText( ) );
		fmx::TextAutoPtr decimal_point;
		decimal_point->Assign ( "." );
		bool integer = fmx_text->Find ( *decimal_point, 1 ) == fmx::Text::kSize_End;
		
		if ( integer ) {
			json_value = json_integer ( number->AsFloat ( ) ); // AsLong returns an int (which overflows)
		} else {
//			json_value = json_real ( number->AsFloat ( ) ); // has rounding problems
			std::string json_text = TextAsUTF8String ( *fmx_text );
//			double real_value = atof ( json_text.c_str() );
//			json_value = json_real ( real_value ); // better (why?) but still has rounding problems
			
			// put it in as string and rip out the unwanted quotes below
			json_value = json_string ( json_text.c_str() );
			is_real = true;
		}
		
	} else if ( data_type == fmx::Data::kDTText ) {
		
		fmx::TextAutoPtr fmx_text;
		fmx_text->SetText ( value.GetAsText( ) );
		std::string json_text = TextAsUTF8String ( *fmx_text );
		json_value = json_string ( json_text.c_str() );

	} else {
		
		// fmx::Data::kDTBinary
		// fmx::Data::kDTInvalid
		
		throw BEJSON_Exception ( kBE_JSON_InvalidDataTypeError );

	}
	
	json_t * json = json_object ( );
	int result = json_object_set ( json, key->c_str(), json_value );
	if ( result ) {
		throw BEJSON_Exception ( result );
	}
	
	size_t flags = JSON_COMPACT;
	char * text = json_dumps ( json, flags );
	StringAutoPtr out ( new std::string ( text ) );
	be_free ( text );
	
	// workaround for rounding/precision errors in reals
	if ( is_real ) {
		size_t found = out->find ( "\":\"" );
		out->erase ( found + 2, 1 );
		out->erase ( out->size() - 1 );
	}

	return out;
	
} // encode



