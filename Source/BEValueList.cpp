/*
 BEValueList.cpp
 BaseElements Plug-In
 
 Copyright 2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEValueList.h"
#include "BEPluginGlobalDefines.h"


#include <sstream>
#include <set>
#include <algorithm>


#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"


using namespace std;


BEValueList::BEValueList ( const string value_list ) {
		
	boost::split ( values, value_list, boost::is_any_of( FILEMAKER_END_OF_LINE ), boost::token_compress_on );
	
}


string BEValueList::unique ( ) {
	
	set<string> unique;
	std::stringstream text;
	
	for ( vector<string>::iterator it = values.begin() ; it != values.end(); ++it ) {
		
		pair<set<string>::iterator, bool> inserted = unique.insert ( *it );
		
		if ( inserted.second == true ) {
			
			if ( text.rdbuf()->in_avail() > 0 ) {
				text << FILEMAKER_END_OF_LINE;
			}
			text << *it;
		}
		
	}
	
	return text.str();
	
}


string BEValueList::filter_out ( auto_ptr<BEValueList> filter_out ) {
	
	const vector<string> to_filter = filter_out->get_values();
	set<string> filter_set ( to_filter.begin(), to_filter.end() );
	
	std::stringstream text;

	for ( vector<string>::iterator it = values.begin() ; it != values.end(); ++it ) {
		
		pair<set<string>::iterator, bool> inserted = filter_set.insert ( *it );
		
		if ( inserted.second == true ) {

			if ( text.rdbuf()->in_avail() > 0 ) {
				text << FILEMAKER_END_OF_LINE;
			}
			text << *it;
			filter_set.erase ( *it );
		}
		
	}
	
	return text.str();
	
}


string BEValueList::sort ( ) {
	
	vector<string> sorted ( values.begin(), values.end() );
	std::sort ( sorted.begin(), sorted.end() );
	
	std::stringstream text;

	for ( vector<string>::iterator it = sorted.begin() ; it != sorted.end(); ++it ) {
		
		if ( text.rdbuf()->in_avail() > 0 ) {
			text << FILEMAKER_END_OF_LINE;
		}
		text << *it;
		
	}
	
	return text.str();
	
}



