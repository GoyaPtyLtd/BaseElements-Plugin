/*
 BEZlib.h
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEZLIB_H)
#define BEZLIB_H


#include "BEPluginUtilities.h"


long UnZip ( const StringAutoPtr archive );
long Zip ( const StringAutoPtr filename );


#endif // BEZLIB_H
