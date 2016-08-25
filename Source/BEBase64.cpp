/*
 BEBase64.cpp
 BaseElements Plug-In
 
 Copyright 2014-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEBase64.h"


#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "boost/archive/iterators/insert_linebreaks.hpp"
#include "boost/archive/iterators/remove_whitespace.hpp"


#include "boost/algorithm/string.hpp"


using namespace std;
using namespace boost::archive::iterators;


typedef base64_from_binary<
	transform_width<char *, 6, 8>
> base64_text;

typedef transform_width<
	binary_from_base64<
		remove_whitespace<string::const_iterator>
	>, 8, 6
> base64_binary;



const vector<char> Base64_Decode ( const std::string& text )
{
	vector<char> out;
	
	try {
		
		string url_encoded = text;

		// throws if we do not lop off any padding
		boost::algorithm::trim_if ( url_encoded, boost::algorithm::is_any_of ( " \t\f\v\n\r" ) );
		boost::algorithm::trim_right_if ( url_encoded, boost::algorithm::is_any_of ( L"=" ) );
		
		// if we have base64url convert it to base64
		string::iterator it = url_encoded.begin();
		while ( it < url_encoded.end() ) {
			switch ( *it ) {
				case '-':
					*it = '+';
					break;
				case '_':
					*it = '/';
					break;
				case ',': // not in rfc4648 but seen in the wild
					*it = '=';
					break;
				default:
					; // do nothing
			}
			++it;
		}
		
		// decode it...
		vector<char> data ( base64_binary ( url_encoded.begin() ), base64_binary ( url_encoded.end() ) );
		out = data;
		
	} catch ( dataflow_exception& e ) { // invalid_base64_character
		g_last_error = e.code;
	}
	
	return out;
	
} // Base64_Decode



const std::string Base64_Encode ( const vector<char> data, const bool base64url )
{
	std::string base64 ( base64_text(&data[0]), base64_text(&data[0] + data.size()) );
	
	string padding = "=";
		
	// if we need base64url convert it
	if ( base64url ) {
			
		// not in rfc4648 but seen in the wild
		//			padding = ",";
			
		string::iterator it = base64.begin();
		while ( it < base64.end() ) {
			switch ( *it ) {
				case '+':
					*it = '-';
					break;
				case '/':
					*it = '_';
					break;
				default:
					; // do nothing
			}
			++it;
		}
			
	}
		
	for ( FMX_UInt32 i = 0 ; i < (base64.length() % 4) ; i ++ ) {
		base64.append ( padding );
	}
		
	return base64;
		
} // Base64_Encode



