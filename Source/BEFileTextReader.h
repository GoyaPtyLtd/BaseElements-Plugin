/*
 BEFileTextReader.h
 BaseElements Plug-In
 
 Copyright 2013-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */



#if !defined(BEFILETEXTREADER_H)
#define BEFILETEXTREADER_H


#include "BEPluginUtilities.h"
#include "BEXMLTextReader.h"


class BEFileTextReader : public BEXMLTextReader {
	
public:
	
	BEFileTextReader ( const std::filesystem::path path ) : BEXMLTextReader ( path ) {}

	
	std::string name();
	std::string get_attribute ( const std::string attribute_name );
	std::string name_attribute () { return this->get_attribute ( "name" ); }
	std::string type_attribute () { return this->get_attribute ( "type" ); }

	bool is_xml();
	bool overwrite();
	bool wanted();

};



#endif // BEFILETEXTREADER_H

