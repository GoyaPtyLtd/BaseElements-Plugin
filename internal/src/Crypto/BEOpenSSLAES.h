/*
 BEOpenSSLAES.h
 BaseElements Plug-In
 
 Copyright 2014-2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEOpenSSLAES__
#define __BaseElements__BEOpenSSLAES__


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"

const std::vector<unsigned char> HexOrContainer ( const fmx::DataVect& parameters, const fmx::uint32 which = 0 );

void GenerateKeyAndInputVector ( const std::string password, std::string& key, std::vector<char>& input_vector );
const std::vector<char> Encrypt_AES ( const std::string key, const std::string text, const std::vector<char> input_vector );
const std::vector<char> Decrypt_AES ( const std::string key, const std::vector<char> text, const std::vector<char> input_vector );

const std::vector<char> CipherEncrypt ( const std::string cipher_name, const std::vector<unsigned char> data, const std::vector<unsigned char> key, const std::vector<unsigned char> iv, const bool padding = true );
const std::vector<char> CipherDecrypt ( const std::string cipher_name, const std::vector<unsigned char> data, const std::vector<unsigned char> key, const std::vector<unsigned char> iv, const bool padding = true );


#endif /* defined(__BaseElements__BEOpenSSLAES__) */
