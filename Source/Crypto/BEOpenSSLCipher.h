/*
 BEOpenSSLCipher.h
 BaseElements Plug-In
 
 Copyright 2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEOpenSSLCipher__
#define __BaseElements__BEOpenSSLCipher__


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"


const std::vector<char> CipherEncrypt ( const std::string cipher_name, const std::vector<unsigned char> data, const std::vector<unsigned char> key, const std::vector<unsigned char> iv, const bool padding );
const std::vector<char> CipherDecrypt ( const std::string cipher_name, const std::vector<unsigned char> data, const std::vector<unsigned char> key, const std::vector<unsigned char> iv, const bool padding );


#endif /* defined(__BaseElements__BEOpenSSLCipher__) */
