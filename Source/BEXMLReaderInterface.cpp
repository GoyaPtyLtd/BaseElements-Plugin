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


const int BEXMLReaderInterface::last_error()
{
	xmlErrorPtr xml_error = xmlGetLastError();
	return xml_error->code;
}

