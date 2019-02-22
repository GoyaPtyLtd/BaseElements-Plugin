/*
 BE_Xero.cpp
 BaseElements Plug-In

 Copyright (c) 2014~2019 Goya. All rights reserved.
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


extern thread_local CustomHeaders g_http_custom_headers;


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

			std::unique_ptr<BEBio> bio ( new BEBio );
			generated_key = bio->extract( private_key );
			generated_key.append ( x509->generate() );

			EVP_PKEY_free ( private_key );

		} else {
			throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
		}

	} else {
		throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
	}

	return generated_key;

} // xero_generate_key_pair


int BEXero::sign_url ( string& url, string& post_arguments, const string http_method ) {
	
	int error = kNoError;
	
	// ensure srand is always set [oauth call oauth_sign_url2 will always return the same nonce under FMS on Win]
	srand ( (unsigned)time ( NULL ) );
	
	string sign_this = url;
	if ( ! post_arguments.empty() ) { // if ( HTTP_METHOD_POST == http_method ) ???
		sign_this += "&" + post_arguments;
	}
	
	char ** parameter_array = NULL;
	int parameter_array_count = oauth_split_url_parameters ( sign_this.c_str(), &parameter_array );
	
	oauth_sign_array2_process (
							   &parameter_array_count,
							   &parameter_array,
							   NULL, // < postargs (unused)
							   OA_RSA,
							   http_method.c_str(), // < HTTP method (defaults to "GET")
							   consumer_key.c_str(),
							   consumer_secret.c_str(),
							   consumer_key.c_str(),
							   NULL
							   );
	
	// we split [x_]oauth_ parameters (for use in HTTP Authorization header)
	const char * oauth_header_parameters = oauth_serialize_url_sep ( parameter_array_count, 1, parameter_array, (char *)", ", 6 );
	oauth_free_array ( &parameter_array_count, &parameter_array );
	
	const string authorisation = "OAuth " + string ( oauth_header_parameters );
	be_free ( (char *)oauth_header_parameters );

	g_http_custom_headers [ "Authorization" ] = authorisation;
	
	return error;
	
} // sign_url

