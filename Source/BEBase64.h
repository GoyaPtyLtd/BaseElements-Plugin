/*
 BEBase64.h
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEBase64__
#define __BaseElements__BEBase64__


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"


#include <vector>


std::vector<char> Base64_Decode ( StringAutoPtr text );
StringAutoPtr Base64_Encode ( std::vector<char> data, bool base64url = false );


#endif /* defined(__BaseElements__BEBase64__) */
