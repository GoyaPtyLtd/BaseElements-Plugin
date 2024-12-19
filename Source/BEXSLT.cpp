/*
 BEXSLT.cpp
 BaseElements Plug-In
 
 Copyright 2010-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEXSLT.h"
#include "BEPluginGlobalDefines.h"
#include "BEValueList.h"

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomma"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/format.hpp>
#pragma GCC diagnostic pop

#include <iostream>

#if defined ( FMX_WIN_TARGET )
	#include <fcntl.h>
	#include <stdio.h>
	#include <io.h>
	#include <tchar.h>
#endif


using namespace fmx;
using namespace std;


const string ReportXSLTError ( const xmlChar * url );
void ResetXMLErrors ( void );


int RegisterNamespaces ( xmlXPathContextPtr xpathCtx, const xmlChar* nsList );
const string XPathObjectAsText ( const xmlXPathObjectPtr xpathObj );
const string XPathObjectAsXML ( const xmlDocPtr xml_document, const xmlXPathObjectPtr xpathObj );
const string NodeSetToValueList ( xmlNodeSetPtr node_set );


// globals for error reporting

thread_local string g_last_xslt_error_text;
thread_local errcode g_last_xslt_error;


// custom error handler for libxml/libxslt ... gather errors reported into a global

static void XSLTErrorFunction ( void *context ATTRIBUTE_UNUSED, const char *message, ... )
{
	va_list parameters;
	va_start ( parameters, message );
	const int size = 10240;
	
	try {
		xmlChar buffer[size]; // individual errors are typically < 1k
		
		int error = xmlStrVPrintf ( buffer, size, message, parameters ); // -1 on error
		if ( error != -1 ) {
			g_last_xslt_error_text += reinterpret_cast<char*> ( buffer );
		} else {
			g_last_xslt_error = kLowMemoryError;
		}
		
	} catch ( exception& /* e */ ) {
		// if there's not enough memory to handle the error message then try sending it to stderr 
		vfprintf ( stderr, message, parameters );
		g_last_xslt_error = kLowMemoryError;
	}	
	
	va_end ( parameters );
	
} // xsltErrorFunction


// format an error as per xsltproc

const string ReportXSLTError ( const xmlChar * url )
{
	
	string result_text = g_last_xslt_error_text;

	if ( url != NULL && xmlStrlen ( url ) > 0 ) {

		string format = "no result for %s\n";
		result_text += boost::str ( boost::format ( format ) %url );

	}
	
	g_last_xslt_error_text.clear();
	
	return result_text;
	
} // ReportXSLTError



void InitialiseLibXSLT ( void )
{
	xmlInitParser();

	xsltInit();
	xsltRegisterAllExtras();
	exsltRegisterAll();
	
}


void CleanupLibXSLT ( void )
{
	xmlCleanupParser();
	xsltCleanupGlobals();
}


void ResetXMLErrors ( void )
{
	xmlResetLastError();
	g_last_xslt_error = kNoError;
	g_last_xslt_error_text.clear();
}


////////////////////////////////////////////////////////////////////////////////////////
//	ApplyXSLTInMemory includes code supplied by Magnus Strand, http://www.smartasystem.se/
////////////////////////////////////////////////////////////////////////////////////////

const string ApplyXSLTInMemory ( const string& xml, const string& xslt_as_filemaker_text, const filesystem::path csv_path, const filesystem::path xml_path )
{
	ResetXMLErrors();
	string result;
	
	// parse the stylesheet
	string xslt = xslt_as_filemaker_text;
	ConvertFileMakerEOLs ( xslt ); // otherwise all errors occur on line 1
	const auto options = XML_PARSE_HUGE | XML_PARSE_IGNORE_ENC | XML_PARSE_NOENT | XML_PARSE_DTDLOAD;

	xmlDocPtr xslt_doc = xmlReadDoc ( (xmlChar *) xslt.c_str(), NULL, UTF8, options );
	if ( xslt_doc ) {
		
		xsltStylesheetPtr stylesheet = xsltParseStylesheetDoc ( xslt_doc );
		// xmlFreeDoc ( xslt_doc ); xslt_doc is now owned by stylesheet
		
		if ( stylesheet ) {
			
			// to get the line numbers etc in the error the stylesheet must have a file name
			stylesheet->doc->URL = xmlStrdup ( (xmlChar *)"<FileMaker::Text::XSLT>" );

#if defined ( FMX_WIN_TARGET )
				// aaaarrrgggghhhhhh!!!!!
				auto wide_path = xml_path.wstring();
				int size = WideCharToMultiByte ( CP_UTF8, 0, wide_path.c_str(), -1, NULL, 0, NULL, NULL );
				vector<char> utf8_data ( size );
				auto bytesConverted = WideCharToMultiByte ( CP_UTF8, 0, wide_path.c_str(), -1, utf8_data.data(), (int)utf8_data.size(), NULL, NULL );
				string utf8 ( utf8_data.begin(), utf8_data.end() );
				xmlDocPtr xml_doc = xmlReadDoc ( (xmlChar *)xml.c_str(), utf8.c_str(), UTF8, options );
#else
				xmlDocPtr xml_doc = xmlReadDoc ( (xmlChar *)xml.c_str(), xml_path.string().c_str(), UTF8, options );
#endif

			if ( xml_doc ) {
				
				// let the processor know to use our error handler and options
				xsltTransformContextPtr context = xsltNewTransformContext ( stylesheet, xml_doc );
				xsltSetGenericErrorFunc ( context, XSLTErrorFunction );
				xmlSetGenericErrorFunc ( context, XSLTErrorFunction );
				xsltSetCtxtParseOptions ( context, options );
				
				// apply the stylesheet
				xmlDocPtr xslt_result = xsltApplyStylesheetUser ( stylesheet, xml_doc, NULL, NULL, NULL, context );
				const xmlError *xml_error = xmlGetLastError();
				if ( xslt_result && xml_error == NULL ) {
					
					// save the output
					if ( !csv_path.empty() ) {
							
						FILE * csv_file = FOPEN ( csv_path.c_str(), _TEXT ( "w" ) );
							
						if ( csv_file ) {
							xsltSaveResultToFile ( csv_file, xslt_result, stylesheet );
							fclose ( csv_file );
						}
							
					} else {
							
						xmlBufferPtr buffer = xmlBufferCreate();
						if ( buffer ) {
								
							xmlOutputBufferPtr outputBufPtr = xmlOutputBufferCreateBuffer ( buffer, NULL );
							if ( outputBufPtr ) {
								xsltSaveResultTo ( outputBufPtr, xslt_result, stylesheet );
								result.assign ( (char *)(buffer->content), buffer->use );	// return transformed xml on success
								xmlOutputBufferClose ( outputBufPtr ); // auto reply =
							}

							xmlBufferFree ( buffer );
						}

					}

				} else {
					result = ReportXSLTError ( xml_doc->URL );
				}
				xmlFreeDoc ( xslt_result );
				xmlResetLastError();
				xsltFreeTransformContext ( context );
				
			}
			xmlFreeDoc ( xml_doc );

		}
		xsltFreeStylesheet ( stylesheet );

	}
	
	if ( g_last_xslt_error != kNoError ) {
		// something has gone very wrong
		throw exception();
	}
	
	return result;
	
} // ApplyXSLTInMemory


/**
 * RegisterNamespaces:
 * @xpathCtx:		the pointer to an XPath context.
 * @nsList:		the list of known namespaces in 
 *			"<prefix1>=<href1> <prefix2>=href2> ..." format.
 *
 * Registers namespaces from @nsList in @xpathCtx.
 *
 * Returns 0 on success and a negative value otherwise.
 */

int RegisterNamespaces ( xmlXPathContextPtr xpathCtx, const xmlChar* nsList )
{
    xmlChar* nsListDup;
    xmlChar* prefix;
    xmlChar* href;
    xmlChar* next;
    
    assert(xpathCtx);
    assert(nsList);
	
    nsListDup = xmlStrdup(nsList);
    if (nsListDup == NULL) {
		return -1;	
    }
    
    next = nsListDup; 
    while (next) {
		/* skip spaces */
		while(*next == ' ')
			next++;
		if(*next == '\0')
			break;
		
		/* find prefix */
		prefix = next;
		next = (xmlChar*)xmlStrchr(next, '=');
		if (next == NULL) {
			xmlFree(nsListDup);
			return -1;	
		}
		*next++ = '\0';	
		
		/* find href */
		href = next;
		next = (xmlChar*)xmlStrchr(next, ' ');
		if (next != NULL) {
			*next++ = '\0';	
		}
		
		/* do register namespace */
		if(xmlXPathRegisterNs(xpathCtx, prefix, href) != 0) {
			xmlFree(nsListDup);
			return -1;	
		}
    }
    
    xmlFree ( nsListDup );
    return 0;
}



const string XPathObjectAsText ( const xmlXPathObjectPtr xpathObj )
{
	
	string result;
	unique_ptr<xmlChar> oject_as_string;

	switch ( xpathObj->type ) {
			
		case XPATH_BOOLEAN:
			oject_as_string.reset ( xmlXPathCastBooleanToString ( xpathObj->boolval ) );
			break;
			
		case XPATH_NUMBER:
			oject_as_string.reset ( xmlXPathCastNumberToString ( xpathObj->floatval ) );
			break;
			
		case XPATH_STRING:
			oject_as_string.reset ( xmlStrdup ( xpathObj->stringval ) );
			break;
			
		case XPATH_NODESET:
			// hope it's a string
			oject_as_string.reset ( xmlXPathCastNodeSetToString ( xpathObj->nodesetval ) );
			break;
			
//		case XPATH_UNDEFINED:
//		case XPATH_POINT:
//		case XPATH_RANGE:
//		case XPATH_LOCATIONSET:
//		case XPATH_USERS:
//		case XPATH_XSLT_TREE:
			
		default:
			// we got nothing
			;
			
	}
	
	if ( oject_as_string ) {
		result.assign ( (char*)oject_as_string.get(), (FMX_UInt32)strlen ( (char*)oject_as_string.get() ) );
	}

	return result;
}


const string XPathObjectAsXML ( const xmlDocPtr xml_document, const xmlXPathObjectPtr xpathObj )
{
	string result;
	
	xmlBufferPtr xml_buffer = xmlBufferCreate();
	const xmlError *xml_error = xmlGetLastError();
	
	if ( xml_buffer && xml_error == NULL && xpathObj->nodesetval != NULL ) {
			
		if ( xpathObj->type == XPATH_NODESET && xpathObj->nodesetval->nodeNr > 0 ) {

			xmlNode *node = xpathObj->nodesetval->nodeTab[0];
			
			int xml_buffer_length = xmlNodeDump ( xml_buffer, xml_document, node, 0, true );
			xml_error = xmlGetLastError();
			
			if ( xml_error == NULL ) {
				
				auto node_as_xml = xmlBufferContent ( xml_buffer ); // owned by xml_buffer
				xml_error = xmlGetLastError();
				
				if ( node_as_xml && xml_error == NULL ) {
					result.assign ( (char*)node_as_xml, xml_buffer_length ); // return node set as string on success
				} else {
					result = ReportXSLTError ( xml_document->URL );
				}
				
			} else {
				result = ReportXSLTError ( xml_document->URL );
			} // if ( xml_error == NULL )

		} else if ( xpathObj->type == XPATH_NODESET && xpathObj->nodesetval->nodeNr == 0 ) {
			; // an empty nodeset ... do nothing ... result = ""
		} else {
			result = ReportXSLTError ( xml_document->URL );
		}
		
	} else {
		result = ReportXSLTError ( xml_document->URL );
	}

	xmlBufferFree ( xml_buffer );
	xmlResetLastError();

	return result;

}


const string NodeSetToValueList ( xmlNodeSetPtr node_set )
{
	
	if ( (node_set == NULL) || (node_set->nodeNr == 0) || (node_set->nodeTab == NULL) ) {
		return "";
	}
	
	if ( node_set->nodeNr > 1 ) {
		xmlXPathNodeSetSort ( node_set );
	}
	
	BEValueList<string> value_list;// = BEValueList<string> ( );

	for ( int i = 0; i < node_set->nodeNr; i++ ) {

		const unique_ptr<xmlChar> node_as_string ( xmlXPathCastNodeToString ( node_set->nodeTab[i] ) );

		if ( node_as_string ) {
			
			string new_value = string ( (char*)node_as_string.get(), (FMX_UInt32)strlen ( (char*)node_as_string.get() ) );
			value_list.append ( new_value );

		}

	}

	return value_list.get_as_filemaker_string();
	
}


const string ApplyXPathExpression ( const string& xml, const string& xpath, const string& ns_list, const xmlXPathObjectType xpath_object_type )
{
	ResetXMLErrors();
	string result;
	
	// parse the xml
	const auto options = XML_PARSE_HUGE | XML_PARSE_IGNORE_ENC | XML_PARSE_NOENT | XML_PARSE_DTDLOAD;
	xmlDocPtr doc = xmlReadDoc ( (xmlChar *)xml.c_str(), NULL, UTF8, options );
	if ( doc ) {
		
		xmlXPathContextPtr xpathCtx = xmlXPathNewContext ( doc );
		if ( xpathCtx ) {
			
			if ( ! ns_list.empty() ) {
				RegisterNamespaces ( xpathCtx, (xmlChar *)ns_list.c_str() );
			}
			
			xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression ( (xmlChar *)xpath.c_str(), xpathCtx );
			if ( xpathObj ) {
				
				if ( xpath_object_type == XPATH_NODESET && xpathObj->type == XPATH_NODESET ) {
					result = NodeSetToValueList ( xpathObj->nodesetval );
				} else if ( xpath_object_type == XPATH_NODESET || xpath_object_type == XPATH_STRING ) {
					result = XPathObjectAsText ( xpathObj );
				} else {
					result = XPathObjectAsXML ( doc, xpathObj );
				}
				
			}
			xmlXPathFreeObject ( xpathObj );
			
		}
		xmlXPathFreeContext ( xpathCtx );

	}
	
	const xmlError *xml_error = xmlGetLastError();
	
	if ( xml_error != NULL ) {
		g_last_xslt_error_text = xml_error->message;
		result = ReportXSLTError ( NULL );
	}

	xmlResetLastError();
	xmlFreeDoc ( doc );

	return result;
	
} // ApplyXPathExpression

