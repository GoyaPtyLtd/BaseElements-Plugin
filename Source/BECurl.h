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
#include "BECurlOption.h"

#include "boost/shared_ptr.hpp"

#include <vector>
#include <map>


#include "curl/curl.h"


typedef std::map<std::string, boost::shared_ptr<BECurlOption> > BECurlOptionMap;


class BECurl_Exception : public std::runtime_error {
	
public:
	BECurl_Exception ( const CURLcode _code ) : runtime_error ( "BECurl_Exception" ) { error_code = _code; }
	
	virtual const CURLcode code() const throw()
	{
		return error_code;
	}
	
	private:
	CURLcode error_code;
	
};


typedef std::map<std::string, std::string> CustomHeaders;


struct MemoryStruct {
	char *memory;
	size_t size;
};


struct host_details {
	std::string host;
	std::string port;
	std::string username;
	std::string password;
};


std::vector<char> GetURL ( const std::string url, const std::string filename = "", const std::string username = "", const std::string password = "" );
std::vector<char> HTTP_POST ( const std::string url, const std::string post_parameters, const std::string username = "", const std::string password = "" );
std::vector<char> HTTP_PUT ( const std::string url, const std::string post_parameters, const std::string username = "", const std::string password = "" );
std::vector<char> HTTP_DELETE ( const std::string url, const std::string username = "", const std::string password = "" );


class BECurl {
	
public:
	
	BECurl ( const std::string download_this, const std::string to_file, std::string username, const std::string password, const std::string parameters );
	~BECurl();
	
    std::vector<char> download ( );
    std::vector<char> http_put ( );
    std::vector<char> http_delete ( );
	
	void set_parameters ( );
	void set_username_and_password ( const std::string username, const std::string password );
	
	int response_code ( ) { return http_response_code; }
	std::string response_headers ( ) { return http_response_headers; }
	void set_custom_headers ( CustomHeaders _headers ) { http_custom_headers = _headers; }
	void set_proxy ( struct host_details proxy_server );
	void set_options ( BECurlOptionMap options );
	CURLcode last_error ( ) { return error; }
	
protected:
	
	std::string url;

	std::string filename;
	FILE * upload_file;

	std::string parameters;
	
	CURL *curl;
	struct curl_slist *custom_headers;	
	struct MemoryStruct headers;
	struct MemoryStruct data;
	std::vector<char> result;
	
	int http_response_code;
	CustomHeaders http_custom_headers;
	std::string http_response_headers;
	CURLcode error;
	std::string proxy;
	std::string proxy_login;
	
    void prepare ( );
	void add_custom_headers ( );
	void write_to_memory ( );
	void perform ( );
	void cleanup ( );
	void easy_setopt ( CURLoption option, ... );

};


#endif // BEXBECURL_HSLT_H

