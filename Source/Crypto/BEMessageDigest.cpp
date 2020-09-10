/*
 BEMessageDigest.cpp
 BaseElements Plug-In
 
 Copyright 2011-2020 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEMessageDigest.h"
#include "BEBase64.h"
#include "BEPluginException.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <openssl/engine.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

#include <vector>

#include <boost/algorithm/hex.hpp>


using namespace std;

#pragma mark -
#pragma mark Prototypes
#pragma mark -

EVP_MD message_digest_algorithm ( const unsigned long algorithm );
string encode_digest ( const unsigned char * message_digest, const unsigned int message_digest_length, const unsigned long output_encoding );


#pragma mark -
#pragma mark "Private" Functions
#pragma mark -

// kBE_MessageDigestAlgorithm_MD2 & kBE_MessageDigestAlgorithm_RMD160 are deliberately omitted

EVP_MD message_digest_algorithm ( const unsigned long algorithm )
{
	EVP_MD type;
	switch ( algorithm ) {
			
		case kBE_MessageDigestAlgorithm_MD5:
			type = *EVP_md5();
			break;
			
		case kBE_MessageDigestAlgorithm_MDC2:
			type = *EVP_mdc2();
			break;
			
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
			throw BEPlugin_Exception ( kBE_UnknownAlgorithm );
			break;
	}

	return type;
	
}


string encode_digest ( const unsigned char * message_digest, const unsigned int message_digest_length, const unsigned long output_encoding )
{
	string digest;
	
	if ( output_encoding == kBE_Encoding_Base64 ) {
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
	EVP_MD type = message_digest_algorithm ( algorithm );
	EVP_MD_CTX context;
	EVP_MD_CTX_init ( &context );


	EVP_DigestInit ( &context, &type );
	EVP_DigestUpdate ( &context, message.data(), message.size() );
	
	unsigned char message_digest [ EVP_MAX_MD_SIZE ];
	unsigned int message_digest_length;
	EVP_DigestFinal ( &context, message_digest, &message_digest_length );
	
	string digest = encode_digest ( message_digest, message_digest_length, output_encoding );
	
	return digest;
	
} // message_digest


string HMAC ( const string message, const unsigned long algorithm, const unsigned long output_encoding, const string key, const unsigned long input_encoding )
{
	ENGINE_load_builtin_engines();
	ENGINE_register_all_complete();

	HMAC_CTX hmac_context;
	HMAC_CTX_init ( &hmac_context );

	EVP_MD type = message_digest_algorithm ( algorithm );

	if ( input_encoding == kBE_Encoding_Base64 ) {

        try {

            const std::vector<char> decoded_key = Base64_Decode ( key );
            HMAC_Init_ex ( &hmac_context, &decoded_key[0], (int)decoded_key.size(), &type, NULL );

		} catch ( std::exception& /* e */ ) {
			throw BEPlugin_Exception ( kMessageDigestDecodeKeyError );
		}


		try {
			
			const std::vector<char> decoded_message = Base64_Decode ( message );
			HMAC_Update ( &hmac_context, (const unsigned char *)&decoded_message[0], decoded_message.size() );

		} catch ( std::exception& /* e */ ) {
			throw BEPlugin_Exception ( kMessageDigestDecodeMessageError );
		}

	} else if ( input_encoding == kBE_Encoding_Hex ) {

		try {

			std::vector<char> decoded_key;
			boost::algorithm::unhex ( key, std::back_inserter ( decoded_key ) );
			HMAC_Init_ex ( &hmac_context, &decoded_key[0], (int)decoded_key.size(), &type, NULL );

		} catch ( std::exception& /* e */ ) {
			throw BEPlugin_Exception ( kMessageDigestDecodeKeyError );
		}

		try {

			std::vector<char> decoded_message;
			boost::algorithm::unhex ( message, std::back_inserter ( decoded_message ) );
			HMAC_Update ( &hmac_context, (const unsigned char *)&decoded_message[0], decoded_message.size() );

		} catch ( std::exception& /* e */ ) {
			throw BEPlugin_Exception ( kMessageDigestDecodeMessageError );
		}

	} else {

		HMAC_Init_ex ( &hmac_context, key.data(), (int)key.size(), &type, NULL );
		HMAC_Update ( &hmac_context, (const unsigned char *)message.data(), message.size() );

	}

	unsigned char hmac [ HMAC_MAX_MD_CBLOCK ];
	unsigned int hmac_length;
	HMAC_Final ( &hmac_context, hmac, &hmac_length );

	HMAC_CTX_cleanup ( &hmac_context );

	string digest = encode_digest ( hmac, hmac_length, output_encoding );

	return digest;
	
} // HMAC

