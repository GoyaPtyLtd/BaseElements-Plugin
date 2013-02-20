/*
 BEJSON.h
 BaseElements Plug-In

 Copyright (c) 2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */



#ifndef __BaseElements__BEJSON__
#define __BaseElements__BEJSON__


#include <iostream>
#include <sstream>
#include "BEPluginUtilities.h"


#include "jansson.h"


enum {
	kBE_JSON_LoadDocumentError = 1,
	kBE_JSON_JSONPathDoesNotExistError = 2
};



class BEJSON_Exception : public runtime_error {
	
public:
	BEJSON_Exception ( const int _code ) : runtime_error ( "BEJSON_Exception" ) { error_code = _code; }
	BEJSON_Exception ( const json_error_t _error ) : runtime_error ( "BEJSON_Exception" ) { json_error = _error; error_code = kBE_JSON_LoadDocumentError; }
	
	virtual const int code() const throw() {
		return error_code;
	}
	
	virtual const string description() const throw() {
		
		if ( error_code == kBE_JSON_LoadDocumentError ) {
			
			ostringstream error_description;
			error_description << "Error: " << json_error.text << endl;
			error_description << "Line: " << json_error.line << endl;
			error_description << "Column: " << json_error.column << endl;
			error_description << "Position: " << json_error.position << endl;
//			error_description << "Source: " << json_error.source << endl;	//	is always <buffer>

			return error_description.str();
			
		} else {
			return "";
		}
		
	}
	
private:
	
	int error_code;
	json_error_t json_error;
	
};



class BEJSON {
	
	
public:
	
	BEJSON ( const StringAutoPtr json );
		~BEJSON ( );

	void json_path_query ( const StringAutoPtr json_path, fmx::Data& results );
	
protected:
	
	json_t * document;
	
};


#endif /* defined(__BaseElements__BEJSON__) */

