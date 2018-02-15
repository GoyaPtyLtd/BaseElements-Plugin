/*
 BEBase64.h
 BaseElements Plug-In
 
 Copyright 2014-2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEBase64__
#define __BaseElements__BEBase64__


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"


#include <vector>


const std::vector<char> Base64_Decode ( const std::string& text );
const std::string Base64_Encode ( const std::vector<char> data, const bool base64url = false );
const std::string Base64_Encode ( const std::string to_encode, const bool base64url = false );


#endif /* defined(__BaseElements__BEBase64__) */
