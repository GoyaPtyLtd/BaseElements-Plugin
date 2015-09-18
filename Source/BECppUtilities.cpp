/*
 BECppUtilities.cpp
 BaseElements Plug-In
 
 Copyright 2013-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BECppUtilities.h"

#include <cstdlib>


using namespace std;


void be_free ( void * pointer )
{
	if ( pointer ) {
		free ( pointer );
	}
}


string join ( string first, string second, string delimiter )
{
	
	string joined = "";
	
	if ( !first.empty() ) {
		
		joined = first;
		
		if ( !second.empty() ) {
			joined.append ( delimiter );
			joined.append ( second );
		}
		
	}
	
	return joined;
	
}	//	join_with_colon


