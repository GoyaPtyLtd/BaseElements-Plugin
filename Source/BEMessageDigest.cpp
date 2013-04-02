/*
 BEMessageDigest.cpp
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEMessageDigest.h"


#include <sys/types.h>
#include <sys/stat.h>
#include "openssl/evp.h"
#include "openssl/sha.h"


StringAutoPtr MD5 ( StringAutoPtr message )
{
	unsigned char md5_hash [ EVP_MAX_MD_SIZE ];
	EVP_MD_CTX md5_context;
	unsigned int md5_length;
	
	EVP_DigestInit ( &md5_context, EVP_md5() );
	EVP_DigestUpdate ( &md5_context, message->data(), message->size() );
	EVP_DigestFinal_ex ( &md5_context, md5_hash, &md5_length );
	EVP_MD_CTX_cleanup ( &md5_context );
	
	// convert the digest to hex
	char output [ EVP_MAX_MD_SIZE ];
	unsigned int i = 0;
    for ( i = 0; i < md5_length; i++ ) {
        sprintf ( output + (i * 2), "%02x", md5_hash[i] );
    }
    output[ 2 * md5_length ] = 0;
	

	StringAutoPtr digest ( new std::string ( output ) );
	
	return digest;
	
}


StringAutoPtr SHA256 ( StringAutoPtr message )
{
	unsigned char sha256_hash [ SHA256_DIGEST_LENGTH ];
	SHA256_CTX sha256_context;
	
	SHA256_Init ( &sha256_context );
	SHA256_Update ( &sha256_context, message->data(), message->size() );
	SHA256_Final ( sha256_hash, &sha256_context );

	// convert the digest to hex
	const int size = 2 * SHA256_DIGEST_LENGTH;
	char output [ size + 1 ];
	int i = 0;
    for ( i = 0; i < SHA256_DIGEST_LENGTH; i++ ) {
        sprintf ( output + (i * 2), "%02x", sha256_hash[i] );
    }
    output [ size ] = 0;
	
	
	StringAutoPtr digest ( new std::string ( output ) );
	
	return digest;

}


