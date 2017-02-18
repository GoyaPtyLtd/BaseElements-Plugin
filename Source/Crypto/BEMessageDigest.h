/*
 BEMessageDigest.h
 BaseElements Plug-In
 
 Copyright 2011-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEMESSAGEDIGEST_H)
#define BEMESSAGEDIGEST_H


#include "BEPluginUtilities.h"

#include "openssl/evp.h"


enum message_digest_algorithm {
	kBE_MessageDigestAlgorithm_MD5 = 1,
	kBE_MessageDigestAlgorithm_SHA256 = 2,
	kBE_MessageDigestAlgorithm_MDC2 = 4,
	kBE_MessageDigestAlgorithm_SHA = 6,
	kBE_MessageDigestAlgorithm_SHA1 = 7,
	kBE_MessageDigestAlgorithm_SHA224 = 8,
	kBE_MessageDigestAlgorithm_SHA384 = 9,
	kBE_MessageDigestAlgorithm_SHA512 = 10
};


enum message_digest_encoding {
	kBE_Encoding_None = 0,
	kBE_Encoding_Hex = 1,
	kBE_Encoding_Base64 = 2
};


enum message_digest_error {
	kBE_UnknownAlgorithm = 15000
};


std::string message_digest ( const std::string message, const unsigned long algorithm = kBE_MessageDigestAlgorithm_SHA256, const unsigned long output_encoding = kBE_Encoding_Hex );
std::string HMAC ( const std::string message, const unsigned long algorithm = kBE_MessageDigestAlgorithm_SHA1, const unsigned long output_encoding = kBE_Encoding_Hex, const std::string key = "", const unsigned long input_encoding = kBE_Encoding_None );
std::string PBKDF2_HMAC ( const std::string password, const std::string salt, const unsigned long iterations, const unsigned long hash_length, const unsigned long algorithm, const unsigned long output_encoding, const unsigned long input_encoding );


#endif // BEMESSAGEDIGEST_H

