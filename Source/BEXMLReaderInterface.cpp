/*
 BEXMLReaderInterface.cpp
 BaseElements Plug-In
 
 Copyright 2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


//using namespace std;


#include "BEXMLReaderInterface.h"
#include "BEPluginUtilities.h"

#include <libxml/xmlreader.h>



const int BEXMLReaderInterface::last_error ( const int default_error )
{
	xmlErrorPtr xml_error = xmlGetLastError();
	
	// don't crash if there's no xml_error
	int error;
	if ( xml_error ) {
		error = xml_error->code;
	} else {
		error = default_error;
	}
	
	return error;
}

