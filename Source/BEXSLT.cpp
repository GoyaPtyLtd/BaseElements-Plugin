/*
 BEXSLT.cpp
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
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

#include <libxml/xpathInternals.h>

#include <libxslt/extra.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

#include "boost/format.hpp"

#include <iostream>


using namespace std;
using namespace fmx;


TextAutoPtr ReportXSLTError ( const xmlChar * url );
StringAutoPtr ConvertFileMakerEOLs ( StringAutoPtr in );
void InitialiseLibXSLT ( void );
void CleanupLibXSLT ( void );


int RegisterNamespaces ( xmlXPathContextPtr xpathCtx, const xmlChar* nsList );
TextAutoPtr XPathObjectAsText ( const xmlXPathObjectPtr xpathObj );
TextAutoPtr XPathObjectAsXML ( const xmlDocPtr xml_document, const xmlXPathObjectPtr xpathObj );
void NodeSetToValueList ( xmlNodeSetPtr ns, TextAutoPtr& result );



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
		
	} catch ( exception& /* e */ ) {
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



////////////////////////////////////////////////////////////////////////////////////////
//	The following code originally supplied by Magnus Strand, http://www.smartasystem.se/
////////////////////////////////////////////////////////////////////////////////////////


#if defined(FMX_MAC_TARGET)

/**
 * xmlBufferWrite:
 * @context:  the xmlBuffer
 * @buffer:  the data to write
 * @len:  number of bytes to write
 *
 * Write @len bytes from @buffer to the xml buffer
 *
 * Returns the number of bytes written
 */

static int xmlBufferWrite (void * context, const char * buffer, int len)
{
    int ret;
	
    ret = xmlBufferAdd((xmlBufferPtr) context, (const xmlChar *) buffer, len);
    if (ret != 0)
        return(-1);
    return(len);
}


/**
 * xmlOutputBufferCreateBuffer:
 * @buffer:  a xmlBufferPtr
 * @encoder:  the encoding converter or NULL
 *
 * Create a buffered output for the progressive saving to a xmlBuffer
 *
 * Returns the new parser output or NULL
 */

xmlOutputBufferPtr xmlOutputBufferCreateBuffer(xmlBufferPtr buffer, xmlCharEncodingHandlerPtr encoder)
{
    xmlOutputBufferPtr ret;
	
    if (buffer == NULL) return(NULL);
	
    ret = xmlOutputBufferCreateIO((xmlOutputWriteCallback)
                                  xmlBufferWrite,
                                  (xmlOutputCloseCallback)
                                  NULL, (void *) buffer, encoder);
	
    return(ret);
}

#endif


TextAutoPtr ApplyXSLTInMemory ( StringAutoPtr xml, StringAutoPtr xslt )
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
			
			xmlDocPtr xml_doc = xmlReadDoc ( (xmlChar *)xml->c_str(), NULL, NULL, options );
			
			if ( xml_doc ) {
				
				// let the processor know to use our error handler and options
				xsltTransformContextPtr context = xsltNewTransformContext ( stylesheet, xml_doc );
				xsltSetGenericErrorFunc ( context, XSLTErrorFunction );
				xmlSetGenericErrorFunc ( context, XSLTErrorFunction );
				xsltSetCtxtParseOptions ( context, options );
				
				
				// apply the stylesheet
				xmlDocPtr xslt_result = xsltApplyStylesheetUser ( stylesheet, xml_doc, NULL, NULL, NULL, context );
				xmlErrorPtr xml_error = xmlGetLastError();
				if ( xslt_result && xml_error == NULL ) {
					xmlBuffer* buf = xmlBufferCreate();
					if (buf) {
						xmlOutputBufferPtr outputBufPtr = xmlOutputBufferCreateBuffer(buf, NULL);
						
						// save the output					
						if (outputBufPtr) {
							xsltSaveResultTo(outputBufPtr, xslt_result, stylesheet);
							result->AssignWithLength((char*)(buf->content), buf->use, fmx::Text::kEncoding_UTF8);	// return transformed xml on success
						}
						xmlBufferFree(buf);
					}
					xmlFreeDoc ( xslt_result );
					
				} else {
					
					// there was an error performing the transform
					
					result->AppendText ( *ReportXSLTError ( xml_doc->URL ) );					
				}
				xsltFreeTransformContext ( context );
				xmlFreeDoc ( xml_doc );
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
    
    xmlFree(nsListDup);
    return 0;
}



TextAutoPtr XPathObjectAsText ( const xmlXPathObjectPtr xpathObj )
{
	
	TextAutoPtr result;

	xmlChar* oject_as_string = NULL;
	
	switch ( xpathObj->type ) {
			
		case XPATH_BOOLEAN:
			oject_as_string = xmlXPathCastBooleanToString ( xpathObj->boolval );
			break;
			
		case XPATH_NUMBER:
			oject_as_string = xmlXPathCastNumberToString ( xpathObj->floatval );
			break;
			
		case XPATH_STRING:
			oject_as_string = xmlStrdup ( xpathObj->stringval );
			break;
			
		case XPATH_NODESET:
			// hope it's a string
			oject_as_string = xmlXPathCastNodeSetToString ( xpathObj->nodesetval );
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
		result->AssignWithLength ( (char*)oject_as_string, (FMX_UInt32)strlen ( (char*)oject_as_string ), fmx::Text::kEncoding_UTF8 );
		xmlFree ( oject_as_string );
	}

	return result;
}



TextAutoPtr XPathObjectAsXML ( const xmlDocPtr xml_document, const xmlXPathObjectPtr xpathObj )
{
	TextAutoPtr result;
	
	xmlBufferPtr xml_buffer = xmlBufferCreate();
	xmlErrorPtr xml_error = xmlGetLastError();
	
	if ( xml_buffer && xml_error == NULL ) {
			
		xmlNode *node = xpathObj->nodesetval->nodeTab[0];
		int xml_buffer_length = xmlNodeDump ( xml_buffer, xml_document, node, 0, true );
		xml_error = xmlGetLastError();

		if ( xml_error == NULL ) {
				
			const xmlChar * node_as_xml = xmlBufferContent ( (xmlBufferPtr)xml_buffer );
			xml_error = xmlGetLastError();
				
			if ( node_as_xml && xml_error == NULL ) {
				result->AssignWithLength ( (char*)node_as_xml, xml_buffer_length, fmx::Text::kEncoding_UTF8 );	// return node set as string on success
				xmlFree ( (xmlChar *)node_as_xml );
			} else {
				result->AppendText ( *ReportXSLTError ( xml_document->URL ) );
			}

		} else {
			result->AppendText ( *ReportXSLTError ( xml_document->URL ) );
		}
				
		xmlFree ( xml_buffer );

	} else {
		result->AppendText ( *ReportXSLTError ( xml_document->URL ) );
	}

	return result;
}



TextAutoPtr ApplyXPath ( StringAutoPtr xml, StringAutoPtr xpath, StringAutoPtr nsList, bool as_text )
{
	g_last_xslt_error = kNoError;
	TextAutoPtr result;
	xmlXPathContextPtr xpathCtx = NULL;
	xmlXPathObjectPtr xpathObj = NULL;
	xmlDocPtr doc = NULL;
	
	xmlInitParser();
	
	// parse the xml
	int options = XML_PARSE_HUGE;
	doc = xmlReadDoc ( (xmlChar *)xml->c_str(), NULL, NULL, options );
	if (!doc)
		goto cleanup;
	
	xpathCtx = xmlXPathNewContext(doc);
	if (!xpathCtx)
		goto cleanup;
	
	RegisterNamespaces(xpathCtx, (xmlChar *)nsList->c_str());
	
	xpathObj = xmlXPathEvalExpression((xmlChar *)xpath->c_str(), xpathCtx);
	
	if ( xpathObj ) {
		if ( as_text ) {
			result->SetText ( *(XPathObjectAsText ( xpathObj )) );
		} else {
			result->SetText ( *(XPathObjectAsXML ( doc, xpathObj )) );
		}
	}
	
cleanup:
	if (xpathObj)
		xmlXPathFreeObject(xpathObj);
	if (doc)
		xmlFreeDoc(doc);
	if (xpathCtx)
		xmlXPathFreeContext(xpathCtx);
	
	xmlCleanupParser();
	
	return result;
} // ApplyXPath


void NodeSetToValueList ( xmlNodeSetPtr ns, TextAutoPtr& result )
{
	TextAutoPtr value;
	TextAutoPtr	cr;
	
	cr->Assign("\r");
	
	if ((ns == NULL) || (ns->nodeNr == 0) || (ns->nodeTab == NULL))
		return;
	
	if (ns->nodeNr > 1)
		xmlXPathNodeSetSort(ns);
	for (int i = 0; i < ns->nodeNr; i++) {
		xmlChar* str = xmlXPathCastNodeToString(ns->nodeTab[i]);
		
		if (str) {
			value->AssignWithLength((char*)str, (FMX_UInt32)strlen((char*)str), fmx::Text::kEncoding_UTF8);
			result->AppendText(*value);
			xmlFree(str);
		}
		result->AppendText(*cr);
	}
}


TextAutoPtr ApplyXPathAll ( StringAutoPtr xml, StringAutoPtr xpath, StringAutoPtr nsList )
{
	g_last_xslt_error = kNoError;
	xmlXPathContextPtr xpathCtx = NULL;
	xmlXPathObjectPtr xpathObj = NULL;
	xmlDocPtr doc = NULL;
	TextAutoPtr result;
	
	xmlInitParser();
	
	// parse the xml
	int options = XML_PARSE_HUGE;
	doc = xmlReadDoc ( (xmlChar *)xml->c_str(), NULL, NULL, options );
	if (!doc)
		goto cleanup;
	
	xpathCtx = xmlXPathNewContext(doc);
	if (!xpathCtx)
		goto cleanup;
	
	RegisterNamespaces(xpathCtx, (xmlChar *)nsList->c_str());
	
	xpathObj = xmlXPathEvalExpression((xmlChar *)xpath->c_str(), xpathCtx);
	
	if ( xpathObj ) {
		
		if ( xpathObj->type == XPATH_NODESET ) {
			TextAutoPtr valueList;
			NodeSetToValueList(xpathObj->nodesetval, valueList);
			result->SetText(*valueList);
		} else {
			result->SetText ( *(XPathObjectAsText ( xpathObj )) );
		}
	}
	
cleanup:
	if (xpathObj)
		xmlXPathFreeObject(xpathObj);
	if (doc)
		xmlFreeDoc(doc);
	if (xpathCtx)
		xmlXPathFreeContext(xpathCtx);
	
	xmlCleanupParser();
	
	return result;
} // ApplyXPathAll

///////////////////////////////////////////////////////////////////////

