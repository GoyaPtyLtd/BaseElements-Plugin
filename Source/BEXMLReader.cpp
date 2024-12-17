/*
 BEXMLReader.cpp
 BaseElements Plug-In
 
 Copyright 2012-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEXMLReader.h"
#include "BEXMLTextReader.h"
#include "BEXMLTextWriter.h"
#include "BEFileTextReader.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>

#include <boost/algorithm/string.hpp>


using namespace std;


int StripXMLNodes ( const string xml, const filesystem::path output_file, const vector<string> node_names )
{

	try {
		
		unique_ptr<BEXMLTextReader> reader ( new BEXMLTextReader ( xml ) );
		shared_ptr<BEXMLTextWriter> writer ( new BEXMLTextWriter ( output_file ) );
		
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
					writer->write_raw ( reader->outer_xml() );
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
	
} // StripXMLNodes


int SplitBEXMLFiles ( const filesystem::path input_file )
{
	
	try {
		
		shared_ptr<BEFileTextReader> reader ( new BEFileTextReader ( input_file ) );
		
		reader->read();
		
		while ( !reader->end() ) {
			
			if ( reader->wanted() ) {
				
				// write it out
				
				try {
					
					auto output_path = input_file;
					output_path.remove_filename() /= reader->name_attribute ();
					
					string type = reader->type_attribute();
					output_path.replace_extension ( type );
					
					ios_base::openmode mode = reader->overwrite() ? ios_base::trunc : ios_base::app;
					ofstream output_file ( output_path, ios_base::out | mode );
					output_file.exceptions ( ofstream::badbit | ofstream::failbit );
					
					string payload;
					
					if ( reader->is_xml() ) {
						payload = reader->inner_xml();
					} else {
						payload = reader->as_string();
					}

					output_file.write ( payload.c_str(), payload.size() );
					output_file.close();

				} catch ( filesystem::filesystem_error& e ) {
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
	
} // SplitBEXMLFiles

