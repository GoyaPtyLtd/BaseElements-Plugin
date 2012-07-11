/*
 BEXMLTextReader.h
 BaseElements Plug-In
 
 Copyright 2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */



#if !defined(BEXMLTEXTREADER_H)
#define BEXMLTEXTREADER_H


#include "BEPluginUtilities.h"
#include "BEXMLReaderInterface.h"


#include <libxml/xmlreader.h>



// Partial wrapper for the libxml2 XMLTextReader interface to simplify error handling


class BEXMLTextReader : public BEXMLReaderInterface {
	
public:
	
	BEXMLTextReader ( const string path );
	~BEXMLTextReader();
	
	void read ( );
	bool end ( ) { return last_node; };

	string name();
	int node_type();
	int depth();
	
	bool has_attributes();
	int number_of_attributes();
	void move_to_element();
	void move_to_attribute ( const int attribute_number );
	string get_attribute ( const string attribute_name );
	bool empty();
	string value();
	
protected:
	
	bool last_node;
	xmlTextReaderPtr reader;
		
};



#endif // BEXMLTEXTREADER_H
