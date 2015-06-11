/*
 BEOpenSSLAES.h
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEOpenSSLAES__
#define __BaseElements__BEOpenSSLAES__


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"


void GenerateKeyAndInputVector ( const std::string password, std::string& key, std::vector<char>& input_vector );
const std::vector<char> Encrypt_AES ( const std::string key, const std::string text, const std::vector<char> input_vector );
const std::vector<char> Decrypt_AES ( const std::string key, const std::vector<char> text, const std::vector<char> input_vector );


#endif /* defined(__BaseElements__BEOpenSSLAES__) */
