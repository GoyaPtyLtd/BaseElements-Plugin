/*
 BEWStringVector.cpp
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEWStringVector.h"

#include <string>
#include <algorithm>


void BEWStringVector::PushBack ( wstring item )
{
	// only add an item if it's not already present
	
	if ( find ( contents.begin(), contents.end(), item ) == contents.end() ) {
		contents.push_back ( item );
	}
}


TextAutoPtr BEWStringVector::AsValueList()
{
	wstring value_list = L"";
	
	vector<wstring>::iterator it = contents.begin();
	
	while ( it != contents.end() ) {
		
		value_list += *it;
		++it;
		
		if ( it != contents.end() ) {
			value_list += L"\r";
		}
	}
	
	TextAutoPtr result;
	result->AssignWide ( value_list.c_str() );
	
	return result;
	
} // value_list
