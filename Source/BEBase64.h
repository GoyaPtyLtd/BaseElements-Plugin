/*
 BEBase64.h
 BaseElements Plug-In
 
 Copyright 2014-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEBase64__
#define __BaseElements__BEBase64__


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"


#include <vector>


std::vector<char> Base64_Decode ( StringAutoPtr text );
std::vector<char> Base64_Decode ( const std::string& text );
StringAutoPtr Base64_Encode ( const std::vector<char> data, const bool base64url = false );


#endif /* defined(__BaseElements__BEBase64__) */
