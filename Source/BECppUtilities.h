/*
 BECppUtilities.h
 BaseElements Plug-In
 
 Copyright 2013-2020 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#ifndef __BaseElements__BECppUtilities__
#define __BaseElements__BECppUtilities__


#include <map>
#include <vector>
#include <string>


void be_free ( void * pointer );
std::string join ( std::string first, std::string second, std::string delimiter = ":" );


template<typename T>
const std::vector<T> map_values ( std::map<T, T> map )
{
	std::vector<T> values;
	
	for ( const auto &key_value_pair : map ) {
		values.push_back ( key_value_pair.second );
	}
	
	return values;

}

#endif /* defined(__BaseElements__BECppUtilities__) */
