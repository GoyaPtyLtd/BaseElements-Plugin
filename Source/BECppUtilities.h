/*
 BECppUtilities.h
 BaseElements Plug-In
 
 Copyright 2013-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#ifndef __BaseElements__BECppUtilities__
#define __BaseElements__BECppUtilities__


#include <string>


void be_free ( void * pointer );
std::string join ( std::string first, std::string second, std::string delimiter = ":" );


#endif /* defined(__BaseElements__BECppUtilities__) */
