/*
 BEFileSystem.cpp
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEFileSystem.h"


using namespace std;
using namespace boost::filesystem;


bool recursive_directory_copy ( const path & from, const path & to  ) {
	
//	if ( !exists ( from ) ) return false;
	
	path base = from;

	if ( is_directory ( from ) ) {

		copy_directory ( from, to );

		directory_iterator end_itr; // default construction yields past-the-end
		
		for ( directory_iterator itr ( from ); itr != end_itr; ++itr ) {
			
			path relative = from;
			
			path target = to;
			target /= itr->path().filename();
			
			bool failed = recursive_directory_copy ( itr->path(), target );
			if ( !failed ) {
				return false;
			}

		}
		
	} else {
		
		// note: aliases & symlinks are not handled correctly
		
		copy ( from, to );
	}
	
	return true;
	
}

