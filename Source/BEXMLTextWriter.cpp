/*
 BEXMLTextWriter.cpp
 BaseElements Plug-In
 
 Copyright 2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEXMLTextWriter.h"


#include <algorithm>
#include <string>


using namespace std;


#pragma mark -
#pragma mark Constructors
#pragma mark -


BEXMLTextWriter::BEXMLTextWriter ( const string path )
{

	writer = xmlNewTextWriterFilename ( path.c_str(), 0 );
	
	if ( writer == NULL ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	
	int return_code = xmlTextWriterStartDocument ( writer, "1.0", UTF8, NULL );
	if ( return_code != kBE_XMLReaderError ) {
		return_code = xmlTextWriterSetIndent ( writer, 0 ); // indent off gives smaller files
	}
	
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
	
}



BEXMLTextWriter::~BEXMLTextWriter()
{
//	return_code = xmlTextWriterEndDocument ( writer );
	xmlTextWriterEndDocument ( writer );
	xmlFreeTextWriter ( writer );
}


#pragma mark -
#pragma mark Methods
#pragma mark -


void BEXMLTextWriter::start_element( const string name )
{
	int return_code = xmlTextWriterStartElement	( writer, (xmlChar *)name.c_str() );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}

}


void BEXMLTextWriter::attribute ( const string name, const string value )
{
	int return_code = xmlTextWriterWriteAttribute ( writer, (xmlChar *)name.c_str(), (xmlChar *)value.c_str() );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}


void BEXMLTextWriter::end_element()
{
	int return_code = xmlTextWriterEndElement ( writer );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}


void BEXMLTextWriter::write_string ( const string to_write )
{
	int return_code = xmlTextWriterWriteString ( writer, (xmlChar *)to_write.c_str() );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}


void BEXMLTextWriter::write_cdata ( const string to_write )
{
	int return_code = xmlTextWriterWriteCDATA ( writer, (xmlChar *)to_write.c_str() );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}


void BEXMLTextWriter::write_raw ( const string to_write )
{
	int return_code = xmlTextWriterWriteRaw ( writer, (xmlChar *)to_write.c_str() );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}
