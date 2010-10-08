/*
 BEXSLT.cpp
 BaseElements Plug-In
 
 Copyright 2010 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEXSLT.h"
#include "BEPluginGlobalDefines.h"
#include "BEMacFunctions.h"

#include <libxml/xmlexports.h>
#include <libxml/xmlstring.h>
#include <libxml/xmlerror.h>
#include <libxml/globals.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlIO.h>

#include <libxslt/extra.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

#include "boost/format.hpp"

#include <iostream>


TextAutoPtr ReportXSLTError ( const xmlChar * url );
StringAutoPtr ConvertFileMakerEOLs ( StringAutoPtr in );
void InitialiseLibXSLT ( void );
void CleanupLibXSLT ( void );


// globals for error reporting

TextAutoPtr g_last_xslt_error_text;
errcode g_last_xslt_error;


// custom error handler for libxml/libxslt ... gather errors reported into a global

static void XSLTErrorFunction ( void *context ATTRIBUTE_UNUSED, const char *message, ... )
{
	va_list parameters;
	va_start ( parameters, message );
	const int size = 10240;
	
	try {
		xmlChar buffer[size]; // individual errors are typically < 1k
		
		int error = xmlStrVPrintf ( buffer, size, (const xmlChar*)message, parameters ); // -1 on error
		if ( error != -1 ) {
			TextAutoPtr error_message;
			error_message->Assign ( (const char*)buffer, Text::kEncoding_UTF8 );
			g_last_xslt_error_text->AppendText ( *error_message );
		} else {
			g_last_xslt_error = kLowMemoryError;
		}
		
	} catch ( exception e ) {
		// if there's not enough memory to handle the error message then try sending it to stderr 
		vfprintf ( stderr, message, parameters );
		g_last_xslt_error = kLowMemoryError;
	}	
	
	va_end ( parameters );
	
} // xsltErrorFunction


// format an error as per xsltproc

TextAutoPtr ReportXSLTError ( const xmlChar * url )
{		
	TextAutoPtr	result_text;
	result_text->AppendText ( *g_last_xslt_error_text );

	string format = "no result for %s\n";
	string message = boost::str ( boost::format ( format ) %url );
	TextAutoPtr no_result_for;
	no_result_for->Assign ( message.c_str(), Text::kEncoding_UTF8 );
	result_text->AppendText ( *no_result_for );
	
	g_last_xslt_error_text->Assign ( "" );
	
	return result_text;
	
} // ReportXSLTError


/*
 filemaker terminates lines with \r, libxml/libxslt prefer \n
 convert filemaker line ending in the text to line feeds
 */

StringAutoPtr ConvertFileMakerEOLs ( StringAutoPtr in )
{
    size_t look_here = 0;
	string from = "\r";
	string to = "\n";
    size_t found_here;
	
    while ( (found_here = in->find ( from, look_here )) != string::npos )
    {
		in->replace ( found_here, from.size(), to );
		look_here = found_here + to.size();
    }
	
    return in;
	
} // ConvertFileMakerEOLs



void InitialiseLibXSLT ( void )
{
	xmlInitMemory();
	xsltRegisterAllExtras();
	exsltRegisterAll();
	
	xmlSubstituteEntitiesDefault ( 1 );
	xmlLoadExtDtdDefaultValue = 1;
    xmlLineNumbersDefault ( 1 );
}


void CleanupLibXSLT ( void )
{
	xsltCleanupGlobals();
	xmlCleanupParser();
}


/*
 an old version of libxml2, this has no effect libxml2 < 2.7 but means large text
 nodes can be handled in later versions
 */

#ifndef XML_PARSE_HUGE
	#define XML_PARSE_HUGE 1<<19
#endif


TextAutoPtr ApplyXSLT ( StringAutoPtr xml_path, StringAutoPtr xslt, StringAutoPtr csv_path )
{
	g_last_xslt_error = kNoError;
	TextAutoPtr result;
	
	InitialiseLibXSLT();
		
	// parse the stylesheet
	xslt = ConvertFileMakerEOLs ( xslt ); // otherwise all errors occur on line 1
	int options = XML_PARSE_HUGE;
	xmlDocPtr xslt_doc = xmlReadDoc ( (xmlChar *) xslt->c_str(), NULL, NULL, options );
	
	if ( xslt_doc ) {
		
		xsltStylesheetPtr stylesheet = xsltParseStylesheetDoc ( xslt_doc );
		// xmlFreeDoc ( xslt_doc ); xslt_doc is now owned by stylesheet
		
		if ( stylesheet ) {
			
			// to get the line numbers etc in the error the stylesheet must have a file name
			stylesheet->doc->URL = xmlStrdup ( (xmlChar *)"<FileMaker::Text::XSLT>" );
			
			xmlDocPtr xml = xmlReadFile ( xml_path->c_str(), NULL, options );
			
			if ( xml ) {
				
				// let the processor know to use our error handler and options
				xsltTransformContextPtr context = xsltNewTransformContext ( stylesheet, xml );
				xsltSetGenericErrorFunc ( context, XSLTErrorFunction );
				xmlSetGenericErrorFunc ( context, XSLTErrorFunction );
				xsltSetCtxtParseOptions ( context, options );

				
				// apply the stylesheet
				xmlDocPtr xslt_result = xsltApplyStylesheetUser ( stylesheet, xml, NULL, NULL, NULL, context );
				xmlErrorPtr xml_error = xmlGetLastError();
				if ( xslt_result && xml_error == NULL ) {
					
					result->Assign ( "" );	// return an empty string on success
					
					// save the output					
					FILE * csv_file = fopen ( csv_path->c_str(), "w" );
					if ( csv_file ) {
						xsltSaveResultToFile ( csv_file, xslt_result, stylesheet );
						fclose ( csv_file );						
					}
					
					xmlFreeDoc ( xslt_result );
					
				} else {
					
					// there was an error performing the transform
					
					result->AppendText ( *ReportXSLTError ( xml->URL ) );					
				}
				xsltFreeTransformContext ( context );
				xmlFreeDoc ( xml );
			}
			xsltFreeStylesheet ( stylesheet );
		}
	}
	
	CleanupLibXSLT();
	
	if ( g_last_xslt_error != kNoError ) {
		// something has gone very wrong
		throw exception();
	}
	
	return result;
	
} // BE_ApplyXSLT


