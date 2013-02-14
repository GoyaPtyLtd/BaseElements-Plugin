/*
 BECurl.h
 BaseElements Plug-In
 
 Copyright 2011-2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BECURL_H)
#define BECURL_H


#include "BEPluginUtilities.h"

#include <vector>
#include <map>


// needed to avoid
//	fatal error C1189: #error : WINDOWS.H already included. MFC apps must not #include <windows.h>
// when BECurl.h is included in BEPluginFuncitons.cpp

#if defined(FMX_WIN_TARGET)
	#include "windows.h"
#endif

#include "curl/curl.h"


class BECurl_Exception : public runtime_error {
	
public:
	BECurl_Exception ( const CURLcode _code ) : runtime_error ( "BECurl_Exception" ) { error_code = _code; }
	
	virtual const CURLcode code() const throw()
	{
		return error_code;
	}
	
	private:
	CURLcode error_code;
	
};


typedef map<string, string> CustomHeaders;


struct MemoryStruct {
	char *memory;
	size_t size;
};


struct host_details {
	string host;
	string port;
	string username;
	string password;
};


vector<char> GetURL ( const string url, const string filename, const string username, const string password );
vector<char> HTTP_POST ( const string url, const string post_parameters, const string username, const string password );
vector<char> HTTP_PUT ( const string url, const string post_parameters, const string username, const string password );
vector<char> HTTP_DELETE ( const string url, const string username, const string password );


class BECurl {
	
public:
	
	BECurl ( const string download_this, const string to_file, string username, const string password, const string parameters );
	~BECurl();
	
    vector<char> download ( );
    vector<char> http_put ( );
    vector<char> http_delete ( );
	
	void set_parameters ( );
	void set_username_and_password ( const string username, const string password );
	
	int response_code ( ) { return http_response_code; }
	string response_headers ( ) { return http_response_headers; }
	void set_custom_headers ( CustomHeaders _headers ) { http_custom_headers = _headers; }
	//	void set_proxy ( string proxy_server )  { proxy = proxy_server; };
	void set_proxy ( struct host_details proxy_server );
	CURLcode last_error ( ) { return error; }
	
protected:
	
	string url;

	string filename;
	FILE * upload_file;

	string parameters;
	
	CURL *curl;
	struct curl_slist *custom_headers;	
	struct MemoryStruct headers;
	struct MemoryStruct data;
	vector<char> result;
	
	int http_response_code;
	CustomHeaders http_custom_headers;
	string http_response_headers;
	CURLcode error;
	string proxy;
	string proxy_login;
	
    void prepare ( );
	void add_custom_headers ( );
	void write_to_memory ( );
	void perform ( );
	void cleanup ( );
	void easy_setopt ( CURLoption option, ... );

};


#endif // BEXBECURL_HSLT_H

