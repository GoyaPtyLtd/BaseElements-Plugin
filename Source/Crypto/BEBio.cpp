/*
 BEBio.cpp
 BaseElements Plug-In

 Copyright 2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEBio.h"
#include "BEPluginException.h"

#include <openssl/err.h>
#include <openssl/pem.h>

#include <vector>


BEBio::BEBio ( )
{
	bio = BIO_new ( BIO_s_mem() );

	if ( ! bio ) {
		throw BEPlugin_Exception ( ERR_get_error() );
	}

}


BEBio::~BEBio ( )
{
	BIO_free ( bio );
}


const std::string BEBio::extract ( )
{
	std::string out;

	int result = 0;

	const int length = BIO_pending ( bio );

	std::vector<char> data ( length );
	result = BIO_read ( bio, &data[0], length );

	if ( result > 0 ) {
		out.assign ( data.begin(), data.end() );
	}

	return out;

} // extract


const std::string BEBio::extract ( X509 * x509 )
{
	std::string out;

	if ( PEM_write_bio_X509 ( bio, x509 ) ) {
		out = extract();
	} else {
		throw BEPlugin_Exception ( ERR_get_error() );
	}

	return out;

} // extract


const std::string BEBio::extract ( RSA * rsa_key_pair )
{
	std::string out;

	if ( PEM_write_bio_RSAPrivateKey ( bio, rsa_key_pair, NULL, NULL, 0, NULL, NULL ) ) {
		out = extract();
	} else {
		throw BEPlugin_Exception ( ERR_get_error() );
	}

	return out;

} // extract


const std::string BEBio::extract ( EVP_PKEY * private_key )
{
	std::string out;

	if ( PEM_write_bio_PrivateKey ( bio, private_key, NULL, NULL, 0, NULL, NULL ) ) {
		out = extract();
	} else {
		throw BEPlugin_Exception ( ERR_get_error() );
	}

	return out;

} // extract


