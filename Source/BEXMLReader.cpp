/*
 BEXMLReader.cpp
 BaseElements Plug-In
 
 Copyright 2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEXMLReader.h"
#include "BEXMLTextReader.h"
#include "BEXMLTextWriter.h"


#include <algorithm>
#include <string>


using namespace std;


int StripXMLNodes ( const string input_file, const string output_file, const vector<string> node_names )
{

	try {
		
		BEXMLTextReader * reader = new BEXMLTextReader ( input_file );
		BEXMLTextWriter * writer = new BEXMLTextWriter ( output_file );
		
		reader->read();
		
		while ( !reader->end() ) {
		
			string name = reader->name();
			int type = reader->node_type();

			// Skip over unwanted tags (including subtrees)
			
			bool unwanted = find ( node_names.begin(), node_names.end(), name ) != node_names.end();
			if ( (type == XML_READER_TYPE_ELEMENT) && unwanted ) {
			
				int depth = reader->depth();
				do {
					reader->read();
				} while ( reader->depth() != depth );
			
				reader->read(); // consume the element's end tag
				continue;			
			}
		
			// process the node
		
			switch ( type ) {
				
				case XML_READER_TYPE_ELEMENT:
				
					writer->start_element ( name );
					
					if ( reader->has_attributes() ) {
					
						int count = reader->number_of_attributes();
					
						for ( int i = 0 ; i < count ; i++ ) {
						
							reader->move_to_attribute ( i );
							const string attribute_name = reader->name();
							reader->move_to_element();
							const string attribute_value = reader->get_attribute ( attribute_name );
							writer->attribute ( attribute_name, attribute_value );

						}
					}
				
					if ( reader->empty() ) {
						writer->end_element();
					}

					break;
					
				case XML_READER_TYPE_TEXT:
					writer->write_string ( reader->value() );
					break;
					
				case XML_READER_TYPE_CDATA:
					// don't use reader->value() since it returns the CDATA as text (not as CDATA)
					// and don't use writer->write_cdata since we already have cdata content
					writer->write_raw ( reader->raw_xml() );
					
					break;
					
				case XML_READER_TYPE_END_ELEMENT:
					writer->end_element();
					break;
					
				default:
					break;
			}

			reader->read();
		
		}
	
		// the output may not be fully written unless we clean up
		
		delete writer;
		delete reader;
		
	} catch ( BEXMLReaderInterface_Exception& e ) {
		return e.code();
	}
		
	return kNoError;
	
 }

