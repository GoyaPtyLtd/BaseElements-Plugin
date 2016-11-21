/*
 BEXMLTextWriter.cpp
 BaseElements Plug-In
 
 Copyright 2012-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEXMLTextWriter.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <algorithm>
#include <string>


using namespace std;


#pragma mark -
#pragma mark Constructors
#pragma mark -

BEXMLTextWriter::BEXMLTextWriter ( const boost::filesystem::path path )
{

	file = path;

	memory = xmlBufferCreate();
	writer = xmlNewTextWriterMemory ( memory, 0 );

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
//	xmlTextWriterEndDocument ( writer );
	xmlFreeTextWriter ( writer );
	xmlBufferFree ( memory );

}


#pragma mark -
#pragma mark Methods
#pragma mark -


void BEXMLTextWriter::start_element( const string name )
{
	const int return_code = xmlTextWriterStartElement	( writer, (xmlChar *)name.c_str() );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}

}


void BEXMLTextWriter::attribute ( const string name, const string value )
{
	const int return_code = xmlTextWriterWriteAttribute ( writer, (xmlChar *)name.c_str(), (xmlChar *)value.c_str() );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}


void BEXMLTextWriter::end_element()
{
	const int return_code = xmlTextWriterEndElement ( writer );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}


void BEXMLTextWriter::write_string ( const string to_write )
{
	const int return_code = xmlTextWriterWriteString ( writer, (xmlChar *)to_write.c_str() );
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
	const int return_code = xmlTextWriterWriteRaw ( writer, (xmlChar *)to_write.c_str() );
	if ( return_code == kBE_XMLReaderError ) {
		throw BEXMLReaderInterface_Exception ( last_error() );
	}
}


void BEXMLTextWriter::save ( )
{
	
	try {
		
		boost::filesystem::ofstream output_file ( file, ios_base::out | ios_base::binary );
		output_file.exceptions ( boost::filesystem::ofstream::failbit | boost::filesystem::ofstream::badbit );
		
		xmlTextWriterEndDocument ( writer );
		
		output_file.write ( (const char *) memory->content, memory->use );
		output_file.close();
		
	} catch ( boost::filesystem::ofstream::failure& /* e */ ) {
		// cannot write to the file
		throw BEXMLReaderInterface_Exception ( last_error ( errno ) );
	} catch ( boost::filesystem::filesystem_error& e ) {
		throw BEXMLReaderInterface_Exception ( last_error ( e.code().value() ) );
	}

}
