/*
 BEJSON.h
 BaseElements Plug-In

 Copyright (c) 2013-2016 Goya. All rights reserved.
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
	kBE_JSON_JSONPathDoesNotExistError = 2,
	kBE_JSON_InvalidDataTypeError = 3
};



class BEJSON_Exception : public std::runtime_error {
	
public:
	BEJSON_Exception ( const int _code ) : runtime_error ( "BEJSON_Exception" ) { error_code = _code; }
	BEJSON_Exception ( const json_error_t _error ) : runtime_error ( "BEJSON_Exception" ) { json_error = _error; error_code = kBE_JSON_LoadDocumentError; }
	
	virtual const int code() const throw() {
		return error_code;
	}
	
	virtual const std::string description() const throw() {
		
		if ( error_code == kBE_JSON_LoadDocumentError ) {
			
			std::ostringstream error_description;
			error_description << "Error: " << json_error.text << std::endl;
			error_description << "Line: " << json_error.line << std::endl;
			error_description << "Column: " << json_error.column << std::endl;
			error_description << "Position: " << json_error.position << std::endl;
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
	
	BEJSON ( );
	BEJSON ( const std::string& json );
		~BEJSON ( );

	void json_path_query ( const std::string& json_path, fmx::Data& results );
	void array_size ( fmx::Data& results );
	std::string encode ( const std::string& format, const fmx::Data& value, const std::string& type );
	
protected:
	
	json_t * document;
	
};


#endif /* defined(__BaseElements__BEJSON__) */

