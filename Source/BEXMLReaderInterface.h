/*
 BEXMLReaderInterface.h
 BaseElements Plug-In
 
 Copyright 2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef BASEELEMENTS_BEXMLREADERINTERFACE_H
#define BASEELEMENTS_BEXMLREADERINTERFACE_H


#include "BEPluginUtilities.h"


// Parent class for a partial wrapper for the libxml2 XMLReader/XMLWriter to simplify error handling


class BEXMLReaderInterface_Exception : public runtime_error {
	
public:
	BEXMLReaderInterface_Exception ( const int _code ) : runtime_error ( "BEXMLReaderInterface_Exception" ) { error_code = _code; }
	
	virtual const int code() const throw()
	{
		return error_code;
	}
	
private:
	int error_code;
	
};


class BEXMLReaderInterface {
	
public:
	const int last_error();
	
	enum {
		kBE_XMLReaderError = -1,
		kBE_XMLReaderAttributeNotFoundError = -2
	};
	
};



#endif // BASEELEMENTS_BEXMLREADERINTERFACE_H
