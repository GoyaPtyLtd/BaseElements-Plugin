/*
 BEMessageDigest.h
 BaseElements Plug-In
 
 Copyright 2011-2020 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEMESSAGEDIGEST_H)
#define BEMESSAGEDIGEST_H


#include "BEPluginUtilities.h"

#include "openssl/evp.h"


enum message_digest_algorithm {
	kBE_MessageDigestAlgorithmMD5 = 1,
	kBE_MessageDigestAlgorithmSHA256 = 2,
	kBE_MessageDigestAlgorithmMDC2 = 4,
	kBE_MessageDigestAlgorithmSHA = 6,
	kBE_MessageDigestAlgorithmSHA1 = 7,
	kBE_MessageDigestAlgorithmSHA224 = 8,
	kBE_MessageDigestAlgorithmSHA384 = 9,
	kBE_MessageDigestAlgorithmSHA512 = 10
};


enum message_digest_encoding {
	kBE_EncodingNone = 0,
	kBE_EncodingHex = 1,
	kBE_EncodingBase64 = 2
};


enum message_digest_error {
	kBE_UnknownAlgorithm = 15000
};


std::string message_digest ( const std::string message, const unsigned long algorithm = kBE_MessageDigestAlgorithmSHA256, const unsigned long output_encoding = kBE_EncodingHex );


#endif // BEMESSAGEDIGEST_H

