/*
 BEOpenSSLRSA.h
 BaseElements Plug-In
 
 Copyright 2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEOpenSSLRSA__
#define __BaseElements__BEOpenSSLRSA__


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"


const std::vector<char> SignatureGenerate_RSA ( const std::vector<unsigned char> data, const std::string privateKey, const std::string digestName, std::string privateKeyPassword );
const bool SignatureVerify_RSA ( const std::vector<unsigned char> data, const std::string publicKey, const std::vector<char> signature, const std::string digestName );


#endif /* defined(__BaseElements__BEOpenSSLRSA__) */
