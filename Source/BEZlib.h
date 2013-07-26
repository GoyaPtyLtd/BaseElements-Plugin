/*
 BEZlib.h
 BaseElements Plug-In
 
 Copyright 2011-2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEZLIB_H)
#define BEZLIB_H


#include "BEPluginUtilities.h"


long UnZip ( const StringAutoPtr archive, const StringAutoPtr output_directory );
long Zip ( const StringAutoPtr filename, const StringAutoPtr archive );


#endif // BEZLIB_H
