/*
 BECurl.h
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BECURL_H)
#define BECURL_H


#include "BEPluginUtilities.h"

#include <vector>
#include <map>


typedef map<string, string> CustomHeaders;


vector<char> GetURL ( const StringAutoPtr url, const StringAutoPtr filename, const StringAutoPtr username, const StringAutoPtr password );
vector<char> HTTP_POST ( const StringAutoPtr url, const StringAutoPtr parameters );


class BECurl {
	
	string url;
	string filename;
	string parameters;
	string username;
	string password;
	
	int http_response_code;
	CustomHeaders http_custom_headers;
	string http_response_headers;
	
public:
	
	BECurl ( string download_this, string to_file, string user, string pass );
	BECurl ( string download_this, string with );
	~BECurl();
	
    vector<char> download ( );
	int response_code ( ) { return http_response_code; }
	string response_headers ( ) { return http_response_headers; }
	void set_custom_headers ( CustomHeaders headers ) { http_custom_headers = headers; }
	
};


#endif // BEXBECURL_HSLT_H

