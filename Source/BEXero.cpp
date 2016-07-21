/*
 BE_Xero.cpp
 BaseElements Plug-In

 Copyright (c) 2014~2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEXero.h"
#include "BECurl.h"
#include "BEPluginException.h"
#include "Crypto/BEBio.h"
#include "Crypto/BEX509.h"

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>


using namespace std;


const std::string xero_generate_key_pair ( BEX509 * x509 )
{
	std::string generated_key;
	
	RSA * rsa_key_pair = RSA_generate_key ( 1024, RSA_3, NULL, NULL );
	
	if ( rsa_key_pair ) {

		EVP_PKEY * private_key = EVP_PKEY_new();

		if ( private_key ) {

			EVP_PKEY_assign_RSA ( private_key, rsa_key_pair );

			x509->set_private_key ( private_key );

			std::auto_ptr<BEBio> bio ( new BEBio );
			generated_key = bio->extract( private_key );
			generated_key.append ( x509->generate() );

			EVP_PKEY_free ( private_key );

		} else {
			throw BEPlugin_Exception ( ERR_get_error() );
		}

	} else {
		throw BEPlugin_Exception ( ERR_get_error() );
	}

	return generated_key;

} // xero_generate_key_pair


int BEXero::sign_url ( string& url, string& post_arguments, const string http_method ) {
	
	int error = kNoError;
	
	// ensure srand is always set [oauth call oauth_sign_url2 will always return the same nonce under FMS on Win]
	srand ( (unsigned)time ( NULL ) );

	char * oauth_url;

	if ( http_method == HTTP_METHOD_POST ) {
		
		// also sign the post_arguments
		string url_to_sign = url + "&" + post_arguments;
		
		char * post_args = NULL;
		oauth_url = oauth_sign_url2 ( url_to_sign.c_str(), &post_args, OA_RSA, http_method.c_str(), consumer_key.c_str(), consumer_secret.c_str(), consumer_key.c_str(), NULL );

		if ( post_args ) {
			post_arguments = post_args;
			be_free ( post_args );
		}
		
	} else { // GET, POST, DELETE
		oauth_url = oauth_sign_url2 ( url.c_str(), NULL, OA_RSA, http_method.c_str(), consumer_key.c_str(), consumer_secret.c_str(), consumer_key.c_str(), NULL );
	}

	if ( oauth_url ) {
		url = oauth_url;
		be_free ( oauth_url );
	} else {
		error = kBE_OAuth_SignURLFailedError;
	}
	
	return error;
	
} // sign_url

