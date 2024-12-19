/*
 BEXMLTextReader.cpp
 BaseElements Plug-In

 Copyright 2012-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "win/BEWinFunctions.h"
#include "BEXMLTextReader.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>


#include <libxml/xmlreader.h>

#if defined ( FMX_WIN_TARGET )
	#include <fcntl.h>
	#include <stdio.h>
	#include <io.h>
#endif


using namespace std;


#pragma mark -
#pragma mark Constructors
#pragma mark -


BEXMLTextReader::BEXMLTextReader ( const string xml )
{
	initialise();

	const auto options = XML_PARSE_HUGE | XML_PARSE_IGNORE_ENC | XML_PARSE_NOENT | XML_PARSE_DTDLOAD;
			
	// if the input, excluding whitespace, begins with < we treat it as xml... if not, treat it as a file path
	auto it = find_if_not ( xml.begin(), xml.end(), [](int c){ return isspace(c); } );
	if ( "<" == xml.substr ( 0, 1 ) || *it == '<' ) {

		file = "XML";
		auto xml_to_parse = xml;
		ConvertFileMakerEOLs ( xml_to_parse );
		xml_text = xmlStrdup ( (const xmlChar *)xml_to_parse.c_str() );

		reader = xmlReaderForDoc ( xml_text, file.string().c_str(), xmlGetCharEncodingName ( XML_CHAR_ENCODING_UTF8 ), options );

	} else {

#if defined ( FMX_WIN_TARGET )
			file = utf8toutf16 ( xml  );
#else
			file = xml;
#endif
		
		file.make_preferred();
		auto file_exists = filesystem::exists ( file );
		if ( file_exists ) {

#if defined ( FMX_WIN_TARGET )
			file_descriptor = _wopen ( file.c_str(), O_RDONLY | _O_WTEXT );
			reader = xmlReaderForFd ( file_descriptor, NULL, NULL, options );
#else
			reader = xmlReaderForFile ( file.c_str(), NULL, options );
#endif

		} else {
			throw BEXMLReaderInterface_Exception ( kNoSuchFileOrDirectoryError );
		}

	}

	if ( NULL != reader ) {
		xml_document = xmlTextReaderCurrentDoc ( reader );
	} else {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}

	
} // ::BEXMLTextReader


BEXMLTextReader::~BEXMLTextReader()
{
	xmlTextReaderClose ( reader );
	xmlFreeTextReader ( reader );

	xmlFree ( (void *)xml_text );
	xmlFreeDoc ( xml_document );

#if defined ( FMX_WIN_TARGET )
	if ( file_descriptor ) {
		_close ( file_descriptor );
	}
#endif

	xmlCleanupParser();

} // ~BEXMLTextReader


void BEXMLTextReader::initialise ( void )
{
	LIBXML_TEST_VERSION; // initialise libxml

	last_node = false;
	error_report = "";
	file_descriptor = 0;
	xml_text = NULL;
}


#pragma mark -
#pragma mark Methods
#pragma mark -


void BEXMLTextReader::read ( )
{
	const int ok = xmlTextReaderRead ( reader );
	if ( ok == 0 ) {
		last_node = true;
	} else if ( ok == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}

}


void BEXMLTextReader::error_reader ( void * arg, const char * msg, xmlParserSeverities /* severity */, xmlTextReaderLocatorPtr locator )
{
	ostringstream error;

	const xmlChar * uri = xmlTextReaderLocatorBaseURI ( locator );
	if ( uri ) {
		error << uri;
		xmlFree ( (void *)uri );
	} else {
		filesystem::path path = *((filesystem::path *)arg);
		error << path.string();
	}

	error << " at line ";
	error << xmlTextReaderLocatorLineNumber ( locator );
	error << " ";
	error << msg;
	error << endl;

	error_report.append ( error.str() );
}


string BEXMLTextReader::parse ( )
{

	xmlTextReaderSetErrorHandler ( reader, (xmlTextReaderErrorFunc) BEXMLTextReader::error_reader, &file );

	int result = 1;

	while ( result == 1 ) {
		result = xmlTextReaderRead ( reader );
	}

	return error_report;

} // validate


string BEXMLTextReader::name()
{
	const xmlChar * node_name = xmlTextReaderName ( reader );
	string name = "";
	if ( node_name ) {
		name = (const char *)node_name;
		xmlFree ( (xmlChar *)node_name );
	} else {
		throw BEXMLReaderInterface_Exception ( last_error ( kBE_XMLReaderAttributeNotFoundError ) );
	}

	return name;

}


int BEXMLTextReader::node_type()
{
	const int node_type = xmlTextReaderNodeType ( reader );
	if ( node_type == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return node_type;
}


int BEXMLTextReader::depth()
{
	const int depth = xmlTextReaderDepth ( reader );
	if ( depth == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return depth;
}


bool BEXMLTextReader::has_attributes()
{
	const int has_attributes = xmlTextReaderHasAttributes ( reader );
	if ( has_attributes == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return has_attributes == 1;
}


int BEXMLTextReader::number_of_attributes()
{
	const int count = xmlTextReaderAttributeCount ( reader );
	if ( count == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return count;
}


void BEXMLTextReader::move_to_element()
{
	const int return_code = xmlTextReaderMoveToElement ( reader );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}


string BEXMLTextReader::get_attribute ( const string attribute_name )
{
	const xmlChar * attribute_value = xmlTextReaderGetAttribute ( reader, (xmlChar *)attribute_name.c_str() );
	string value;
	if ( attribute_value ) {
		value = (const char *)attribute_value;
		xmlFree ( (xmlChar *)attribute_value );
	} else {
		throw BEXMLReaderInterface_Exception ( last_error ( kBE_XMLReaderAttributeNotFoundError ) );
	}

	return value;
}



void BEXMLTextReader::move_to_attribute ( const int attribute_number )
{
	const int return_code = xmlTextReaderMoveToAttributeNo ( reader, attribute_number );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	} else if ( return_code == 0 ) {
		throw BEXMLReaderInterface_Exception ( kBE_XMLReaderAttributeNotFoundError );
	}
}


bool BEXMLTextReader::empty()
{
	const int empty = xmlTextReaderIsEmptyElement ( reader );
	if ( empty == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return empty == 1;
}


string BEXMLTextReader::value()
{
	const xmlChar * reader_value = xmlTextReaderValue ( reader );
	string value;

	if ( reader_value ) {
		value = (const char *)reader_value;
		xmlFree ( (void *)reader_value );
	}

	return value;

}


string BEXMLTextReader::inner_xml()
{
	string inner_xml;

	const xmlChar * raw_xml = xmlTextReaderReadInnerXml ( reader );
	const xmlError *xml_error = xmlGetLastError();
	if ( NULL == xml_error ) {
		inner_xml = (char *)raw_xml;
		xmlFree ( (void *)raw_xml );
	} else {
		auto error_code = xml_error->code;
		xmlResetLastError();

		throw BEXMLReaderInterface_Exception ( error_code );
	}

	return inner_xml;

} // inner_xml


string BEXMLTextReader::outer_xml()
{
	string outer_xml;

	const xmlChar * raw_xml = xmlTextReaderReadOuterXml ( reader );
	const xmlError *xml_error = xmlGetLastError();
	if ( NULL == xml_error ) {
		outer_xml = (char *)raw_xml;
		xmlFree ( (void *)raw_xml );
	} else {
		auto error_code = xml_error->code;
		xmlResetLastError();
		
		throw BEXMLReaderInterface_Exception ( error_code );
	}

	return outer_xml;

} // outer_xml


string BEXMLTextReader::content()
{
	const xmlChar * xml_data = xmlNodeGetContent ( xmlTextReaderCurrentNode ( reader ) );
	const string xml_result ( (char *)xml_data, xmlStrlen ( xml_data ) );
	xmlFree ( (xmlChar *)xml_data );

	return xml_result;
} // content


string BEXMLTextReader::as_string()
{
	string value;

	const xmlChar * reader_value = xmlTextReaderReadString ( reader );
	if ( reader_value ) {
		value = (const char *)reader_value;
		xmlFree((void *)reader_value);
	}

	return value;
} // as_string


void BEXMLTextReader::skip_unwanted_nodes ( )
{
	// Skip over unwanted tags (including subtrees)

	const int depth = this->depth();

	do {
		this->read();
	} while ( this->depth() != depth );

	// consume the element's end tag

	if ( this->node_type() == XML_READER_TYPE_END_ELEMENT ) {
		this->read();
	}

} // skip_unwanted_nodes

