/*
 BEOAuth.h
 BaseElements Plug-In
 
 Copyright 2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEOAuth__
#define __BaseElements__BEOAuth__

#include <iostream>

#include "BEPluginUtilities.h"


enum {
	kBE_OAuth_TokensNotFoundError = 1,
	kBE_OAuth_SignURLFailedError = 2,
	kBE_OAuth_HTTPRequestFailedError = 3
};


class BEOAuth {


	public:
				
		BEOAuth ( const string key, const string secret );
		
		int oauth_request ( const string uri, string key = "", string secret = "" );
		int sign_url ( string& url, string& post_arguments );
	
		string get_request_key ( void ) { return request_key; }
		string get_request_secret ( void ) { return request_secret; }

		string get_access_key ( void ) { return access_key; }
		string get_access_secret ( void ) { return access_secret; }
		
	protected:
		
		int parse_reply ( const string reply, string& key, string& secret );
		string http_request ( const string url, const string post_arguments );
	
		string consumer_key;
		string consumer_secret;
		
		string request_key;
		string request_secret;

		string access_key;
		string access_secret;

};


#endif /* defined(__BaseElements__BEOAuth__) */
