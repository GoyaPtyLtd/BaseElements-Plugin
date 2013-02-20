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
#include "BEPluginUtilities.h"


#include "jansson.h"



class BEJSON_Exception : public runtime_error {
	
public:
	BEJSON_Exception ( const int _code ) : runtime_error ( "BEJSON_Exception" ) { error_code = _code; }
	
	virtual const int code() const throw()
	{
		return error_code;
	}
	
private:
	int error_code;
	
};



class BEJSON {
	
	
public:
	
	BEJSON ( const StringAutoPtr json );
		~BEJSON ( );

	void json_path_query ( const StringAutoPtr json_path, fmx::Data& results );
	const int last_error();
	
	enum {
		kBE_JSON_LoadDocumentError = 1,
		kBE_JSON_JSONPathDoesNotExistError = 2
	};
	
protected:
	
	json_t * document;
	
};


#endif /* defined(__BaseElements__BEJSON__) */

