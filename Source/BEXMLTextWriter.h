/*
 BEXMLTextWriter.h
 BaseElements Plug-In
 
 Copyright 2012-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEXMLTEXTWRITER_H)
#define BEXMLTEXTWRITER_H


#include "BEPluginUtilities.h"
#include "BEXMLReaderInterface.h"

#include "boost/filesystem/path.hpp"

#include <libxml/xmlwriter.h>


// Partial wrapper for the libxml2 XMLTextWriter interface to simplify error handling


class BEXMLTextWriter : public BEXMLReaderInterface {
	
public:
	
	BEXMLTextWriter ( const boost::filesystem::path path );
	~BEXMLTextWriter();

	void start_element ( const std::string name );
	void attribute ( const std::string name, const std::string value );
	void end_element ( );
	void write_string ( const std::string to_write );
	void write_cdata ( const std::string to_write );
	void write_raw ( const std::string to_write );

	void save ( );

protected:
	
	boost::filesystem::path file;

	xmlTextWriterPtr writer;
	xmlBufferPtr memory;

};



#endif // BEXMLTEXTWRITER_H
