/*
 BEOAuth.cpp
 BaseElements Plug-In
 
 Copyright 2013-2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEOAuth.h"
#include "BECurl.h"


#include <boost/scoped_ptr.hpp>


using namespace std;


BEOAuth::BEOAuth ( const string key, const string secret ) {
	
	consumer_key = key;
	consumer_secret = secret;
//	replace ( consumer_secret.begin(), consumer_secret.end(), FILEMAKER_END_OF_LINE_CHAR, '\n' );
	
}



BEOAuth::~BEOAuth ( ) {};



int BEOAuth::parse_reply ( const string reply, string& key, string& secret ) {

	int error = kNoError;

	char **tokens = NULL;
	
	int number_of_parameters = oauth_split_url_parameters ( reply.c_str(), &tokens );
	qsort ( tokens, number_of_parameters, sizeof(char *), oauth_cmpstringp );
	
	if ( number_of_parameters == 2 && !strncmp ( tokens[0], "oauth_token=", 11 ) && !strncmp ( tokens[1], "oauth_token_secret=", 18 ) ) {
		key = &( tokens[0][12] );
		secret = &( tokens[1][19] );
	} else {
		error = kBE_OAuth_TokensNotFoundError;
	}

	be_free ( tokens );

	return error;

} // parse_reply



string BEOAuth::http_request ( const string url, const string post_arguments ) {

	vector<char> response;
	
	try {

		if ( post_arguments.empty() ) { // no post arguments? make a GET request
			BECurl curl = BECurl ( url, kBE_HTTP_METHOD_GET );
			response = curl.download ( );
		} else {
			BECurl curl = BECurl ( url, kBE_HTTP_METHOD_POST, "", "", "", post_arguments );
			response = curl.download ( );
		}
		
	} catch ( BECurl_Exception& /* e */ ) {
		;// error = e.code(); // we return an empty string on error
	}
	
	const string reply ( response.begin(), response.end() );
		
	return reply;

} // http_request


int BEOAuth::sign_url ( string& url, string& post_arguments, const std::string /* http_method */ ) {
	
	int error = kNoError;
	
	char * post_args = NULL;
	char * oauth_url = oauth_sign_url2 ( url.c_str(), &post_args, OA_HMAC, NULL, consumer_key.c_str(), consumer_secret.c_str(), access_key.c_str(), access_secret.c_str() );
	
	if ( oauth_url ) {
		
		url = oauth_url;
		post_arguments = post_args;
		
		be_free ( oauth_url );
		be_free ( post_args );
		
	} else {
		error = kBE_OAuth_SignURLFailedError;
	}
	
	return error;
	
} // sign_url



int BEOAuth::oauth_request ( const string uri, string key, string secret ) {
	
	int error = kNoError;
	
	char * token_key = key.empty() ? NULL : (char *)key.c_str();
	char * token_secret = secret.empty() ? NULL : (char *)secret.c_str();
	char * post_arguments = NULL;
	
	char * oauth_url = oauth_sign_url2 ( uri.c_str(), &post_arguments, OA_HMAC, NULL, consumer_key.c_str(), consumer_secret.c_str(), token_key, token_secret );
	
	if ( oauth_url ) {
		
		string reply = http_request ( oauth_url, post_arguments );
		
		be_free ( oauth_url );
		
		if ( !reply.empty() ) {
			
			if ( token_key ) {
				error = parse_reply ( reply, access_key, access_secret );
			} else {
				error = parse_reply ( reply, request_key, request_secret );
			}
			
		} else {
			error = kBE_OAuth_HTTPRequestFailedError;
		}
		
	} else {
		error = kBE_OAuth_SignURLFailedError;
	}
	
	be_free ( post_arguments );
	
	return error;
	
} // oauth_request

