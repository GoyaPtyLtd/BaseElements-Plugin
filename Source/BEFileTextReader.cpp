/*
 BEFileTextReader.cpp
 BaseElements Plug-In
 
 Copyright 2013-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEFileTextReader.h"


#include <boost/algorithm/string.hpp>


// over-rides


std::string BEFileTextReader::name()
{
	std::string name = "";

	try {
		name = BEXMLTextReader::name();
	} catch ( BEXMLReaderInterface_Exception& e ) {
		if ( e.code() == kBE_XMLReaderAttributeNotFoundError ) {
			; // we ok with this
		} else {
			throw;
		}
	}
	
	return name;
}


std::string BEFileTextReader::get_attribute ( const std::string attribute_name )
{
	std::string value = "";
	
	try {
		value = BEXMLTextReader::get_attribute ( attribute_name );
	} catch ( BEXMLReaderInterface_Exception& e ) {
		if ( e.code() == kBE_XMLReaderAttributeNotFoundError ) {
			; // we ok with this
		} else {
			throw;
		}
	}
	
	return value;
}


// subclass

bool BEFileTextReader::is_xml()
{
	return boost::iequals (  this->type_attribute (), "xml" );
} // is_xml


bool BEFileTextReader::overwrite()
{
	return boost::iequals ( this->get_attribute ( "overwrite" ), "true" );	
} // overwrite


bool BEFileTextReader::wanted()
{
	return boost::iequals ( this->name(), "baseelements:file" );
} // wanted



