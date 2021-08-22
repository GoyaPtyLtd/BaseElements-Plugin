/*
 BEMessageDigest.cpp
 BaseElements Plug-In
 
 Copyright 2011-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEMessageDigest.h"
#include "BEBase64.h"
#include "BEPluginException.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <openssl/engine.h>
#include <openssl/sha.h>

#include <vector>

#include <boost/algorithm/hex.hpp>


using namespace std;

#pragma mark -
#pragma mark Prototypes
#pragma mark -

const EVP_MD * message_digest_algorithm ( const unsigned long algorithm );
string encode_digest ( const unsigned char * message_digest, const unsigned int message_digest_length, const unsigned long output_encoding );


#pragma mark -
#pragma mark "Private" Functions
#pragma mark -

// kBE_MessageDigestAlgorithm_MD2 & kBE_MessageDigestAlgorithm_RMD160 are deliberately omitted

const EVP_MD * message_digest_algorithm ( const unsigned long algorithm )
{
	const EVP_MD * type = NULL;
	switch ( algorithm ) {
			
		case kBE_MessageDigestAlgorithmMD5:
			type = EVP_md5();
			break;
			
		case kBE_MessageDigestAlgorithmMDC2:
			type = EVP_mdc2();
			break;
			
		case kBE_MessageDigestAlgorithmSHA:
		case kBE_MessageDigestAlgorithmSHA1:
			type = EVP_sha1();
			break;
			
		case kBE_MessageDigestAlgorithmSHA224:
			type = EVP_sha224();
			break;
			
		case kBE_MessageDigestAlgorithmSHA256:
			type = EVP_sha256();
			break;
			
		case kBE_MessageDigestAlgorithmSHA384:
			type = EVP_sha384();
			break;
			
		case kBE_MessageDigestAlgorithmSHA512:
			type = EVP_sha512();
			break;
			
		default:
			throw BEPlugin_Exception ( kBE_UnknownAlgorithm );
			break;
	}

	return type;
	
}


string encode_digest ( const unsigned char * message_digest, const unsigned int message_digest_length, const unsigned long output_encoding )
{
	string digest;
	
	if ( output_encoding == kBE_EncodingBase64 ) {
		std::vector<char> data ( message_digest, message_digest + message_digest_length );
		digest = Base64_Encode ( data );
	} else {
		boost::algorithm::hex ( message_digest, message_digest + message_digest_length, std::back_inserter ( digest ) );
	}
	
	return digest;

} // encode_digest


#pragma mark -
#pragma mark "Public" Functions
#pragma mark -

string message_digest ( const string message, const unsigned long algorithm, const unsigned long output_encoding )
{
	const EVP_MD * type = message_digest_algorithm ( algorithm );
	EVP_MD_CTX * context = EVP_MD_CTX_new();

	EVP_DigestInit ( context, type );
	EVP_DigestUpdate ( context, message.data(), message.size() );
	
	unsigned char message_digest [ EVP_MAX_MD_SIZE ];
	unsigned int message_digest_length;
	EVP_DigestFinal ( context, message_digest, &message_digest_length );
	EVP_MD_CTX_free ( context );
	
	string digest = encode_digest ( message_digest, message_digest_length, output_encoding );
	
	return digest;
	
} // message_digest

