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

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>


using namespace std;


const std::string xero_generate_key ( RSA * rsa_key_pair, const bool generate_private_key );


const std::string xero_generate_key ( RSA * rsa_key_pair, const bool generate_private_key )
{
	string out;

	int result = 0;
	BIO * key = BIO_new ( BIO_s_mem() );

	if ( key ) {

		if ( generate_private_key ) {
			result = PEM_write_bio_RSAPrivateKey ( key, rsa_key_pair, NULL, NULL, 0, NULL, NULL );
		} else {
			result = PEM_write_bio_RSAPublicKey ( key, rsa_key_pair );
		}

		if ( result ) {

			const int key_length = BIO_pending ( key );

			vector<char> new_key ( key_length );
			result = BIO_read ( key, &new_key[0], key_length );

			if ( result > 0 ) {
				out.assign ( new_key.begin(), new_key.end() );
			}

		}

		BIO_free ( key );

	}

	if ( key == NULL || result <= 0 ) {
		throw BEPlugin_Exception ( ERR_get_error() );
	}
	
	return out;
	
} // xero_generate_key


const std::string xero_generate_key_pair ( )
{
	std::string generated_key;

	RSA * rsa_key_pair = RSA_generate_key ( 1024, RSA_3, NULL, NULL );
	if ( rsa_key_pair ) {

		try {

			generated_key = xero_generate_key ( rsa_key_pair, true );
			generated_key.append ( xero_generate_key ( rsa_key_pair, false ) );

			RSA_free ( rsa_key_pair );

		} catch ( BEPlugin_Exception& e ) {
			RSA_free ( rsa_key_pair );
			throw;
		}

	} else {
		throw BEPlugin_Exception ( ERR_get_error() );
	}

	return generated_key;

} // xero_generate_key_pair


int BEXero::sign_url ( string& url, string& post_arguments, const string http_method ) {
	
	int error = kNoError;
	
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

