/*
 BECppUtilities.cpp
 BaseElements Plug-In
 
 Copyright 2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BECppUtilities.h"


void be_free ( void * pointer )
{
	if ( pointer ) {
		free ( pointer );
	}
}

