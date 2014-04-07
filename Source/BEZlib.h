/*
 BEZlib.h
 BaseElements Plug-In
 
 Copyright 2011-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEZLIB_H)
#define BEZLIB_H


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"
#include "BEValueList.h"


const long UnZip ( const StringAutoPtr archive, const StringAutoPtr output_directory );
const long Zip ( const BEValueList * filename, const StringAutoPtr archive );


#endif // BEZLIB_H
