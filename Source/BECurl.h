/*
 BECurl.h
 BaseElements Plug-In
 
 Copyright 2011-2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BECURL_H)
#define BECURL_H


#include "BEPluginUtilities.h"
#include "BECurlOption.h"

#include <vector>

#include "curl/curl.h"


typedef enum be_http_method {
	kBE_HTTP_METHOD_DELETE,
	kBE_HTTP_METHOD_GET,
	kBE_HTTP_METHOD_POST,
	kBE_HTTP_METHOD_PUT,
	kBE_HTTP_METHOD_PATCH,
	kBE_FTP_METHOD_UPLOAD,
	kBE_FTP_METHOD_DELETE
} be_http_method;


#define HTTP_METHOD_DELETE "DELETE"
#define HTTP_METHOD_GET "GET"
#define HTTP_METHOD_POST "POST"
#define HTTP_METHOD_PUT "PUT"
#define HTTP_METHOD_PATCH "PATCH"


size_t ReadMemoryCallback ( void *ptr, size_t size, size_t nmemb, void *data );


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
	char * memory;
	size_t size;
	char * origin;
};


struct host_details {
	std::string host;
	std::string port;
	std::string username;
	std::string password;
};



class BECurl {
	
public:
	
	BECurl ( );
	
	BECurl ( const std::string download_this, const be_http_method method = kBE_HTTP_METHOD_GET, const boost::filesystem::path to_file = "", const std::string username = "", const std::string password = "", const std::string post_parameters = "", const std::vector<char> put_data = std::vector<char>() );
	
	~BECurl();
	
	void Init ( );
	
    std::vector<char> download ( );
    std::vector<char> http_put ( );
    std::vector<char> http_delete ( );
    std::vector<char> ftp_upload ( );
    std::vector<char> ftp_delete ( );
	
	be_http_method get_http_method ( ) { return http_method; };
	void set_http_method ( be_http_method method ) { http_method = method; };
	
	void set_parameters ( );
	void set_custom_headers ( CustomHeaders _headers );
	void set_username_and_password ( );
	
	int response_code ( ) { return http_response_code; }
	std::string response_headers ( ) { return http_response_headers; }
	void set_proxy ( struct host_details proxy_server );
	void set_options ( BECurlOptionMap options );
	CURLcode last_error ( ) { return error; }
	
protected:
	
	CURL *curl;

	std::string url;
	
	std::string username;
	std::string password;
	
	be_http_method http_method;

	boost::filesystem::path filename;
	FILE * upload_file;
	
	std::vector<char> upload_data;
	struct MemoryStruct userdata;
	struct curl_mime * mime;

	std::string parameters;
	
	struct curl_slist * custom_headers;
	struct curl_slist * command_list;
	struct MemoryStruct headers;
	struct MemoryStruct data;
	std::vector<char> result;
	
	int http_response_code;
	std::string http_response_headers;
	CURLcode error;

	void write_to_memory ( );
	void perform ( );

	void easy_setopt ( CURLoption option, ... );
	void configure_progress_dialog ( );
	
	std::string http_method_as_string ( );
	
	void prepare_data_upload ( );
	void prepare_upload ( );
	
};


#endif // BEXBECURL_HSLT_H

