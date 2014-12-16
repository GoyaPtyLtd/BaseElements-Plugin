//
//  BE_Xero.cpp
//  BaseElements
//
//  Created by Mark Banks on 14/01/2014.
//  Copyright (c) 2014 Goya. All rights reserved.
//

#include "BEXero.h"
#include "BECurl.h"



using namespace std;



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

