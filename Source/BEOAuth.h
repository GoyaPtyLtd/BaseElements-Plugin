/*
 BEOAuth.h
 BaseElements Plug-In
 
 Copyright 2013-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEOAuth__
#define __BaseElements__BEOAuth__


#include "BEPluginUtilities.h"

#include "oauth.h"

#include <boost/algorithm/string.hpp>


enum {
	kBE_OAuth_TokensNotFoundError = 1,
	kBE_OAuth_SignURLFailedError = 2,
	kBE_OAuth_HTTPRequestFailedError = 3
};


class BEOAuth {


	public:
				
		BEOAuth ( const std::string key, const std::string secret );
		virtual ~BEOAuth ( );
		
		int oauth_request ( const std::string uri, std::string key = "", std::string secret = "" );
		virtual int sign_url ( std::string& url, std::string& post_arguments, const std::string http_method );
	
		std::string get_request_key ( void ) { return request_key; }
		std::string get_request_secret ( void ) { return request_secret; }

		std::string get_access_key ( void ) { return access_key; }
		std::string get_access_secret ( void ) { return access_secret; }
	
		std::string get_last_error ( void ) { return last_error; }
	
	protected:
		
		int parse_reply ( const std::string reply, std::string& key, std::string& secret );
		std::string http_request ( const std::string url, const std::string post_arguments );
	
		std::string consumer_key;
		std::string consumer_secret;
		
		std::string request_key;
		std::string request_secret;

		std::string access_key;
		std::string access_secret;
	
		std::string last_error;

};


#endif /* defined(__BaseElements__BEOAuth__) */
