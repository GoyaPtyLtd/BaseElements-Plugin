/*
 BEXMLSchema.cpp
 BaseElements Plug-In
 
 Copyright (c) 2017-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEXMLSchema.h"
#include "BEPluginGlobalDefines.h"

#include <libxml/parser.h>
#include <libxml/xmlschemas.h>


// globals for error reporting

thread_local std::string g_xsd_errors;


static void XSDErrorFunction ( void *context ATTRIBUTE_UNUSED, const char *message, ... )
{
	va_list parameters;
	va_start ( parameters, message );
	const int size = 10240;
	
	try {
		xmlChar buffer[size]; // individual errors are typically < 1k
		
		int error = xmlStrVPrintf ( buffer, size, message, parameters ); // -1 on error
		if ( error != -1 ) {
			g_xsd_errors += (const char *)buffer;
		} else {
			g_last_error = kLowMemoryError;
		}
		
	} catch ( std::exception& /* e */ ) {
		// if there's not enough memory to handle the error message then try sending it to stderr
		vfprintf ( stderr, message, parameters );
		g_last_error = kLowMemoryError;
	}
	
	va_end ( parameters );
	
} // xsdErrorFunction


const std::string validate_xml ( const std::string xml, const std::string schema )
{
	g_xsd_errors.clear( );
	
	xmlInitMemory();	
	xmlSubstituteEntitiesDefault ( 1 );
	xmlLoadExtDtdDefaultValue = 1;
	xmlLineNumbersDefault ( 1 );
	
	const int options = XML_PARSE_HUGE;

	xmlDocPtr schema_document = xmlReadDoc ( (xmlChar *)schema.c_str(), NULL, NULL, options );

	if ( schema_document ) {

		xmlSchemaParserCtxtPtr parser_context = xmlSchemaNewDocParserCtxt ( schema_document );
		if ( parser_context ) {
		
			xmlSetGenericErrorFunc ( parser_context, XSDErrorFunction );

			xmlSchemaPtr xml_schema = xmlSchemaParse ( parser_context );
			if ( xml_schema ) {
				
				xmlSchemaValidCtxtPtr valid_context = xmlSchemaNewValidCtxt ( xml_schema );
				if ( valid_context ) {

					xmlSetGenericErrorFunc ( valid_context, XSDErrorFunction );
					xmlSchemaSetValidErrors ( valid_context, &XSDErrorFunction, &XSDErrorFunction, NULL ); // void =
					xmlDocPtr xml_document = xmlReadDoc ( (xmlChar *) xml.c_str(), NULL, NULL, options );

					if ( xml_document ) {
						xmlSchemaValidateDoc ( valid_context, xml_document ); // 0 if the document is schemas valid, a positive error code number otherwise and -1 in case of internal or API error
						xmlFreeDoc ( xml_document );
					}
					
					xmlSchemaFreeValidCtxt ( valid_context );
				}
				
				xmlSchemaFree ( xml_schema );
			}

			xmlSchemaFreeParserCtxt ( parser_context );
		}
		
		xmlFreeDoc ( schema_document );
	}
	
	xmlCleanupParser();
	
	return g_xsd_errors;

}


