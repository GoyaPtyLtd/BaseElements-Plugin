/*
 BEXMLTextReader.cpp
 BaseElements Plug-In
 
 Copyright 2012-3 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEXMLTextReader.h"


#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>

#include "boost/filesystem.hpp"


#pragma mark -
#pragma mark Constructors
#pragma mark -


BEXMLTextReader::BEXMLTextReader ( const string path )
{
	last_node = false;
	error_report = "";
	
	boost::filesystem::path file = path;
	bool file_exists = boost::filesystem::exists ( file );

	if ( file_exists ) {

		reader = xmlNewTextReaderFilename ( path.c_str() );

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
	xmlFreeTextReader ( reader );
	if ( xml_document ) {
		xmlFreeDoc ( xml_document );
	}
	
}


#pragma mark -
#pragma mark Methods
#pragma mark -


void BEXMLTextReader::read ( )
{
	int ok = xmlTextReaderRead ( reader );
	if ( ok == 0 ) {
		last_node = true;
	} else if ( ok == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	
}


void BEXMLTextReader::error_reader (void * /* arg */, const char * msg, xmlParserSeverities /* severity */, xmlTextReaderLocatorPtr locator )
{
	ostringstream error;

	error << xmlTextReaderLocatorBaseURI ( locator ) << " at line ";
	error << xmlTextReaderLocatorLineNumber ( locator ) << std::endl;
	error << msg << std::endl;

	error_report.append ( error.str() );
}


string BEXMLTextReader::parse ( )
{
		
	xmlTextReaderSetErrorHandler ( reader, (xmlTextReaderErrorFunc) BEXMLTextReader::error_reader, 0 );
	
	int result = 1;

	while ( result == 1 ) {
		result = xmlTextReaderRead ( reader );
	}

	return error_report;

} // validate


string BEXMLTextReader::name()
{
	const xmlChar * node_name = xmlTextReaderName ( reader );
	string name = (const char *)node_name;
	xmlFree ( (xmlChar *)node_name );
	return name;
}


int BEXMLTextReader::node_type()
{
	int node_type = xmlTextReaderNodeType ( reader );
	if ( node_type == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return node_type;
}


int BEXMLTextReader::depth()
{
	int depth = xmlTextReaderDepth ( reader );
	if ( depth == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return depth;
}


bool BEXMLTextReader::has_attributes()
{
	int has_attributes = xmlTextReaderHasAttributes ( reader ) == 1;
	if ( has_attributes == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return has_attributes == 1;
}


int BEXMLTextReader::number_of_attributes()
{
	int count = xmlTextReaderAttributeCount ( reader );
	if ( count == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return count;
}


void BEXMLTextReader::move_to_element()
{
	int return_code = xmlTextReaderMoveToElement ( reader );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}

string BEXMLTextReader::get_attribute ( const string attribute_name )
{
	const xmlChar * attribute_value = xmlTextReaderGetAttribute ( reader, (xmlChar *)attribute_name.c_str() );
	string value = (const char *)attribute_value;
	xmlFree ( (xmlChar *)attribute_value );
	return value;
}

void BEXMLTextReader::move_to_attribute ( const int attribute_number )
{
	int return_code = xmlTextReaderMoveToAttributeNo ( reader, attribute_number );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	} else if ( return_code == 0 ) {
		throw BEXMLReaderInterface_Exception ( kBE_XMLReaderAttributeNotFoundError );
	}
}


bool BEXMLTextReader::empty()
{
	int empty = xmlTextReaderIsEmptyElement ( reader );
	if ( empty == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	return empty == 1;
}


string BEXMLTextReader::value()
{
	const xmlChar * reader_value = xmlTextReaderValue ( reader );
	string value = (const char *)reader_value;
	xmlFree ( (void *)reader_value );
	return value;
}


string BEXMLTextReader::raw_xml()
{
	xmlBufferPtr node_content = xmlBufferCreate();
	xmlOutputBufferPtr node = (xmlOutputBufferPtr) xmlMalloc ( sizeof(xmlOutputBuffer) );
	memset ( node, 0, (size_t) sizeof(xmlOutputBuffer) );
	node->buffer = node_content;
	
	xmlNodeDumpOutput ( node, xml_document, xmlTextReaderCurrentNode ( reader ), 0, true, "UTF-8" );
	const xmlChar * xml_data = xmlBufferContent ( (xmlBufferPtr)node_content );
	size_t xml_length = xmlBufferLength ( node_content );
	string xml_result ( (char *)xml_data, xml_length );
	
	xmlFree ( (xmlChar *)xml_data );
	
	return xml_result;
} // raw_xml

