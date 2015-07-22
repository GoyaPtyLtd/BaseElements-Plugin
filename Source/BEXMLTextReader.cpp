/*
 BEXMLTextReader.cpp
 BaseElements Plug-In
 
 Copyright 2012-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEXMLTextReader.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>

#include "boost/filesystem.hpp"

#if defined ( FMX_WIN_TARGET )
	#include <fcntl.h>
	#include <stdio.h>
	#include <io.h>
#endif

#pragma mark -
#pragma mark Constructors
#pragma mark -

BEXMLTextReader::BEXMLTextReader ( const boost::filesystem::path path )
{
	LIBXML_TEST_VERSION; // initialise libxml
	
	last_node = false;
	error_report = "";
	
	file = path;
	bool file_exists = boost::filesystem::exists ( file );

	if ( file_exists ) {

#if defined ( FMX_WIN_TARGET )
		file_descriptor = _wopen ( file.c_str(), O_RDONLY | _O_WTEXT );
		reader = xmlReaderForFd ( file_descriptor, NULL, NULL, XML_PARSE_HUGE );
#else
		reader = xmlReaderForFile ( file.c_str(), NULL, XML_PARSE_HUGE );
#endif

		if ( reader != NULL ) {
			xml_document = xmlTextReaderCurrentDoc ( reader );
		} else {
			throw BEXMLReaderInterface_Exception ( last_error() );
		}
		
	} else {
		throw BEXMLReaderInterface_Exception ( kNoSuchFileOrDirectoryError );
	}

}


BEXMLTextReader::~BEXMLTextReader()
{
	xmlTextReaderClose ( reader );
	xmlFreeTextReader ( reader );
	
	if ( xml_document ) {
		xmlFreeDoc ( xml_document );
	}
	
#if defined ( FMX_WIN_TARGET )
	if ( file_descriptor ) {
		_close ( file_descriptor );
	}
#endif
	
	xmlCleanupParser();
	
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
	std::ostringstream error;

	const xmlChar * uri = xmlTextReaderLocatorBaseURI ( locator );
	if ( uri ) {
		error << uri;
		xmlFree ( (void *)uri );
	} else {
		boost::filesystem::path path = *((boost::filesystem::path *)arg);
		error << path.string();
	}

	error << " at line ";
	error << xmlTextReaderLocatorLineNumber ( locator );
	error << " ";
	error << msg;
	error << std::endl;

	error_report.append ( error.str() );
}


std::string BEXMLTextReader::parse ( )
{
		
	xmlTextReaderSetErrorHandler ( reader, (xmlTextReaderErrorFunc) BEXMLTextReader::error_reader, &file );
	
	int result = 1;

	while ( result == 1 ) {
		result = xmlTextReaderRead ( reader );
	}

	return error_report;

} // validate


std::string BEXMLTextReader::name()
{
	const xmlChar * node_name = xmlTextReaderName ( reader );
	std::string name = "";
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


std::string BEXMLTextReader::get_attribute ( const std::string attribute_name )
{
	const xmlChar * attribute_value = xmlTextReaderGetAttribute ( reader, (xmlChar *)attribute_name.c_str() );
	std::string value;
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


std::string BEXMLTextReader::value()
{
	const xmlChar * reader_value = xmlTextReaderValue ( reader );
	std::string value;
	
	if ( reader_value ) {
		value = (const char *)reader_value;
		xmlFree ( (void *)reader_value );
	}

	return value;

}


std::string BEXMLTextReader::inner_xml()
{
	std::string inner_xml;

	const xmlChar * raw_xml = xmlTextReaderReadInnerXml ( reader );
	const xmlErrorPtr xml_error = xmlGetLastError();
	if ( NULL == xml_error ) {
		inner_xml = (char *)raw_xml;
		xmlFree ( (void *)raw_xml );
	} else {
		throw BEXMLReaderInterface_Exception ( xml_error->code );
	}

	return inner_xml;
	
} // inner_xml


std::string BEXMLTextReader::outer_xml()
{
	std::string outer_xml;

	const xmlChar * raw_xml = xmlTextReaderReadOuterXml ( reader );
	const xmlErrorPtr xml_error = xmlGetLastError();
	if ( NULL == xml_error ) {
		outer_xml = (char *)raw_xml;
		xmlFree ( (void *)raw_xml );
	} else {
		throw BEXMLReaderInterface_Exception ( xml_error->code );
	}

	return outer_xml;

} // outer_xml


std::string BEXMLTextReader::content()
{
	const xmlChar * xml_data = xmlNodeGetContent ( xmlTextReaderCurrentNode ( reader ) );
	const std::string xml_result ( (char *)xml_data, xmlStrlen ( xml_data ) );
	xmlFree ( (xmlChar *)xml_data );

	return xml_result;
} // content


std::string BEXMLTextReader::as_string()
{
	std::string value;

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

