/*
 BEMessageDigest.cpp
 BaseElements Plug-In
 
 Copyright 2011-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEMessageDigest.h"
#include "BEBase64.h"

#include <sys/types.h>
#include <sys/stat.h>
#include "openssl/sha.h"
#include "openssl/hmac.h"

#include <vector>

#include "boost/algorithm/hex.hpp"


using namespace std;


EVP_MD message_digest_algorithm ( const unsigned long algorithm )
{
	EVP_MD type;
	switch ( algorithm ) {
			
			//		case kBE_MessageDigestAlgorithm_MD2:
			//			type = EVP_m;
			//			break;
			
		case kBE_MessageDigestAlgorithm_MD5:
			type = *EVP_md5();
			break;
			
		case kBE_MessageDigestAlgorithm_MDC2:
			type = *EVP_mdc2();
			break;
			
			//		case kBE_MessageDigestAlgorithm_RMD160:
			//			type = EVP_md;
			//			break;
			
		case kBE_MessageDigestAlgorithm_SHA:
			type = *EVP_sha();
			break;
			
		case kBE_MessageDigestAlgorithm_SHA1:
			type = *EVP_sha1();
			break;
			
		case kBE_MessageDigestAlgorithm_SHA224:
			type = *EVP_sha224();
			break;
			
		case kBE_MessageDigestAlgorithm_SHA256:
			type = *EVP_sha256();
			break;
			
		case kBE_MessageDigestAlgorithm_SHA384:
			type = *EVP_sha384();
			break;
			
		case kBE_MessageDigestAlgorithm_SHA512:
			type = *EVP_sha512();
			break;
			
		default:
			throw 1;
			break;
	}

	return type;
	
}


string encode_digest ( const unsigned char * message_digest, const unsigned int message_digest_length, const unsigned long output_encoding )
{
	string digest;
	
	if ( output_encoding == kBE_Encoding_Base64 ) {
		std::vector<unsigned char> data ( *message_digest, message_digest_length );
		digest = *Base64_Encode ( data );
	} else {
		boost::algorithm::hex ( message_digest, message_digest + message_digest_length, std::back_inserter ( digest ) );
	}
	
	return digest;

} // encode_digest


string message_digest ( const string message, const unsigned long algorithm, const unsigned long output_encoding )
{
	EVP_MD type = message_digest_algorithm ( algorithm );
	EVP_MD_CTX context;
	
	EVP_DigestInit ( &context, &type );
	EVP_DigestUpdate ( &context, message.data(), message.size() );
	
	unsigned char message_digest [ EVP_MAX_MD_SIZE ];
	unsigned int message_digest_length;
	EVP_DigestFinal ( &context, message_digest, &message_digest_length );
	
	string digest = encode_digest ( message_digest, message_digest_length, output_encoding );
	
	return digest;
	
} // message_digest


string HMAC ( const string message, const unsigned long algorithm, const unsigned long output_encoding, const string key )
{
	unsigned char hmac [ HMAC_MAX_MD_CBLOCK ];
	unsigned int hmac_length;
	HMAC_CTX hmac_context;
	
	EVP_MD type = message_digest_algorithm ( algorithm );

	HMAC_Init ( &hmac_context, key.data(), key.size(), &type );
	HMAC_Update ( &hmac_context, (const unsigned char *)message.data(), message.size() );
	HMAC_Final ( &hmac_context, hmac, &hmac_length );
		
	string digest = encode_digest ( hmac, hmac_length, output_encoding );
	
	return digest;
	
} // HMAC

