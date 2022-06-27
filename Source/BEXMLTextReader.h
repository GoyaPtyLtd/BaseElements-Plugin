/*
 BEXMLTextReader.h
 BaseElements Plug-In
 
 Copyright 2012-2022 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */



#if !defined(BEXMLTEXTREADER_H)
#define BEXMLTEXTREADER_H


#include "BEPluginUtilities.h"
#include "BEXMLReaderInterface.h"

#include <boost/filesystem/path.hpp>

#include <libxml/xmlreader.h>


static thread_local std::string error_report;


// Partial wrapper for the libxml2 XMLTextReader interface to simplify error handling

class BEXMLTextReader : public BEXMLReaderInterface {
	
public:
	
	BEXMLTextReader ( const boost::filesystem::path path ) : BEXMLTextReader ( path.string() ) {};
	BEXMLTextReader ( const std::string xml );
	~BEXMLTextReader();
	
	void initialise ( void );

	void read ( );
	bool end ( ) { return last_node; };
	
	std::string parse ( );

	std::string name();
	int node_type();
	int depth();
	
	bool has_attributes();
	int number_of_attributes();
	void move_to_element();
	void move_to_attribute ( const int attribute_number );
	std::string get_attribute ( const std::string attribute_name );
	bool empty();
	std::string value();
	std::string as_string();
	std::string inner_xml();
	std::string outer_xml();
	std::string content();
	void skip_unwanted_nodes();
		
	static void error_reader ( void * arg, const char * msg, xmlParserSeverities severity, xmlTextReaderLocatorPtr locator );
	
protected:
	
	boost::filesystem::path file;
	bool last_node;
	xmlTextReaderPtr reader;
	xmlDocPtr xml_document;
	int file_descriptor;
	const xmlChar * xml_text;

};



#endif // BEXMLTEXTREADER_H
