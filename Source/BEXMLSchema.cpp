/*
 BEXMLSchema.cpp
 BaseElements Plug-In

 Copyright (c) 2017-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEXMLSchema.h"
#include "BEPluginException.h"
#include "BEPluginGlobalDefines.h"

#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

#include <libxml/c14n.h>

#include <sstream>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMException.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/SAX/SAXException.h>
#include <Poco/XML/XMLException.h>
#include <Poco/XML/XMLWriter.h>


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

	const auto options = XML_PARSE_HUGE | XML_PARSE_NOENT | XML_PARSE_DTDLOAD;

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


const std::string canonical_xml ( const std::string xml )
{

	const auto options = XML_PARSE_HUGE | XML_PARSE_NOENT | XML_PARSE_DTDLOAD;

	xmlDocPtr xml_document = xmlReadDoc ( (xmlChar *) xml.c_str(), NULL, NULL, options );

	std::string canonized_xml;

	if ( xml_document ) {

		xmlChar * canonical_xml_document = NULL;
		auto result = xmlC14NDocDumpMemory ( xml_document, NULL, XML_C14N_1_1, NULL, 1, &canonical_xml_document );
		xmlFreeDoc ( xml_document );

		if ( result > 0 ) {
			canonized_xml = (char *) canonical_xml_document;
			xmlFree ( canonical_xml_document );
		} else {
			throw BEPlugin_Exception ( result );
		}

//	} else {
//		throw BEPlugin_Exception ( result );
	}


	xmlCleanupParser();

	return canonized_xml;

}


const std::string pretty_print_xml ( const std::string xml )
{

	Poco::XML::DOMParser parser;
	Poco::XML::AutoPtr<Poco::XML::Document> xml_document;

	try {
		xml_document = parser.parseString ( xml );
	} catch ( Poco::XML::XMLException /* &e */ ) {
		throw BEPlugin_Exception ( kXMLParseError );
	}

	Poco::XML::DOMWriter writer;
	writer.setNewLine ( FILEMAKER_END_OF_LINE );
	writer.setOptions ( Poco::XML::XMLWriter::CANONICAL_XML | Poco::XML::XMLWriter::PRETTY_PRINT | Poco::XML::XMLWriter::PRETTY_PRINT_ATTRIBUTES );

	std::ostringstream out;
	writer.writeNode ( out, xml_document );

	return (std::string)out.str();

} // pretty_print_xml

