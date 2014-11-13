/*
 BEXMLReader.cpp
 BaseElements Plug-In
 
 Copyright 2012-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEXMLReader.h"
#include "BEXMLTextReader.h"
#include "BEXMLTextWriter.h"
#include "BEFileTextReader.h"

#include <algorithm>
#include <string>

#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/scoped_ptr.hpp"


using namespace std;
using namespace boost::filesystem;


int StripXMLNodes ( const path input_file, const path output_file, const vector<string> node_names )
{

	try {
		
		boost::scoped_ptr<BEXMLTextReader> reader ( new BEXMLTextReader ( input_file ) );
		boost::scoped_ptr<BEXMLTextWriter> writer ( new BEXMLTextWriter ( output_file ) );
		
		reader->read();
		
		while ( !reader->end() ) {

			bool wanted = find ( node_names.begin(), node_names.end(), reader->name() ) == node_names.end();
			
			if ( reader->node_type() == XML_READER_TYPE_ELEMENT && !wanted ) {
				reader->skip_unwanted_nodes ( );
			}
			
			// process the node
		
			switch ( reader->node_type() ) {
				
				case XML_READER_TYPE_ELEMENT:
					
					writer->start_element ( reader->name() );
					
					if ( reader->has_attributes() ) {
					
						int count = reader->number_of_attributes();
					
						for ( int i = 0 ; i < count ; i++ ) {
						
							reader->move_to_attribute ( i );
							const string attribute_name = reader->name();
							reader->move_to_element();
							const string attribute_value = reader->get_attribute ( attribute_name );
							writer->attribute ( attribute_name, attribute_value );
							
						} // for
						
					} // if
				
					if ( reader->empty() ) {
						writer->end_element();
					}

					break;

				case XML_READER_TYPE_TEXT:
					writer->write_string ( reader->value() );
					break;
					
				case XML_READER_TYPE_CDATA:
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

		writer->save();
			
	} catch ( BEXMLReaderInterface_Exception& e ) {
		return e.code();
	}
		
	return kNoError;
	
 }


int SplitBEXMLFiles ( const path input_file )
{
	
	try {
		
		boost::scoped_ptr<BEFileTextReader> reader ( new BEFileTextReader ( input_file ) );
		
		reader->read();
		
		while ( !reader->end() ) {
			
			if ( reader->wanted() ) {
				
				// write it out
				
				try {
					
					path output_path = input_file;
					output_path.remove_filename() /= reader->name_attribute ();
					
					string type = reader->type_attribute();
					output_path.replace_extension ( type );
					
					ios_base::openmode mode = reader->overwrite() ? ios_base::trunc : ios_base::app;					
					boost::filesystem::ofstream output_file ( output_path, ios_base::out | mode );
					output_file.exceptions ( boost::filesystem::ofstream::badbit | boost::filesystem::ofstream::failbit );
					
					string payload;
					
					if ( reader->is_xml() ) {
						payload = reader->inner_raw_xml();
					} else {
						payload = reader->as_string();
					}
					
					if ( !payload.empty() ) {
						output_file.write ( payload.c_str(), payload.size() );
						output_file.close();
					}
					
				} catch ( filesystem_error& e ) {
					g_last_error = e.code().value();
				} catch ( exception& /* e */ ) {
					g_last_error = errno; // unable to write to the file
				}
				
			}

			reader->read();
			
		}
				
	} catch ( BEXMLReaderInterface_Exception& e ) {
		return e.code();
	}
	
	return kNoError;
	
}

