//
//  BE_Xero.cpp
//  BaseElements
//
//  Created by Mark Banks on 14/01/2014.
//  Copyright (c) 2014 Goya. All rights reserved.
//

#include "BEXero.h"



using namespace std;



int BEXero::oauth_request ( const string uri/*, string key, string secret */) {
	
	int error = kNoError;
	
	const char * token_key = consumer_key.c_str();
	
	OAuthMethod method = OA_RSA;
	boost::algorithm::replace_all ( consumer_secret, FILEMAKER_END_OF_LINE, "\r\n" );
	const char * token_secret = NULL;
		
	char * oauth_url = oauth_sign_url2 ( uri.c_str(), NULL, method, NULL, consumer_key.c_str(), consumer_secret.c_str(), token_key, token_secret );
	
	if ( oauth_url ) {
		
		string post_args;
		string reply = http_request ( oauth_url, post_args );
		
		be_free ( oauth_url );
		
		last_error = "";
		
		if ( !reply.empty() ) {
			
			if ( token_key ) {
				error = parse_reply ( reply, access_key, access_secret );
			} else {
				error = parse_reply ( reply, request_key, request_secret );
			}
			
			if ( error != 0 ) {
				last_error = reply;
			}
			
		} else {
			error = kBE_OAuth_HTTPRequestFailedError;
		}
		
	} else {
		error = kBE_OAuth_SignURLFailedError;
	}
	
	return error;
	
} // oauth_request



