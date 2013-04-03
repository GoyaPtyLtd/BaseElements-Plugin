/*
 BECurl.cpp
 BaseElements Plug-In
 
 Copyright 2011-2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BECurl.h"
#include "BEPluginGlobalDefines.h"
#include "BEOAuth.h"


#include <errno.h>


#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"

#include <iostream>


using namespace std;
using namespace boost::filesystem;


enum http_methods {
	kBE_HTTP_METHOD_POST,
	kBE_HTTP_METHOD_DELETE,
	kBE_HTTP_METHOD_PUT
};


#pragma mark -
#pragma mark Globals
#pragma mark -

int g_http_response_code;
string g_http_response_headers;

CustomHeaders g_http_custom_headers;
struct host_details g_http_proxy;

extern BEOAuth * g_oauth;

BECurlOptionMap g_curl_options;


#pragma mark -
#pragma mark Functions
#pragma mark -


static size_t WriteMemoryCallback (void *ptr, size_t size, size_t nmemb, void *data )
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;
	
	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		g_last_error = kLowMemoryError;
		exit ( EXIT_FAILURE );
	}
	
	memcpy(&(mem->memory[mem->size]), ptr, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	
	return realsize;
}


static MemoryStruct InitalizeCallbackMemory ( void )
{
	struct MemoryStruct data;

// suppress "Memory is never released; potential leak of memory pointed to by..."
	
#ifndef __clang_analyzer__
	
	data.memory = (char *)malloc(1);  // this is grown as needed by WriteMemoryCallback
	data.memory[0] = '\0'; 
	data.size = 0;
	
	return data;
	
#endif
	
}


static vector<char> PerformAction ( BECurl * curl, int which = kBE_HTTP_METHOD_POST )
{	
	curl->set_proxy ( g_http_proxy );
	curl->set_custom_headers ( g_http_custom_headers );
	
	vector<char>data;
	
	switch ( which ) {
		case kBE_HTTP_METHOD_DELETE:
			data = curl->http_delete ( );
			break;
			
		case kBE_HTTP_METHOD_PUT:
			data = curl->http_put ( );
			break;
			
		default:
			data = curl->download ( );
			break;
	}
	
	g_last_error = curl->last_error();
	g_http_response_code = curl->response_code();
	g_http_response_headers = curl->response_headers();
	g_http_custom_headers.clear();
	
	return data;
	
} // Perform



vector<char> GetURL ( const string url, const string filename, const string username, const string password )
{	
	BECurl * curl = new BECurl ( url, filename, username, password, "" );
	return PerformAction ( curl );
}


vector<char> HTTP_POST ( const string url, const string parameters, const string username, const string password )
{	
	BECurl * curl = new BECurl ( url, "", username, password, parameters );
	return PerformAction ( curl );		
}


vector<char> HTTP_PUT ( const string url, const string filename, const string username, const string password )
{	
	BECurl * curl = new BECurl ( url, filename, username, password, "" );
	return PerformAction ( curl, kBE_HTTP_METHOD_PUT );		
}


vector<char> HTTP_DELETE ( const string url, const string username, const string password )
{	
	BECurl * curl = new BECurl ( url, "", username, password, "" );
	return PerformAction ( curl, kBE_HTTP_METHOD_DELETE );		
}



#pragma mark -
#pragma mark Constructors
#pragma mark -


BECurl::BECurl ( const string download_this, const string to_file, const string username, const string password, const string post_parameters )
{
	url = download_this;
	filename = to_file;
	upload_file = NULL; // must intialise, we crash otherwise
	
	http_response_code = 0;
	
	// set up curl as much as we can
	
	error = curl_global_init ( CURL_GLOBAL_ALL );
	if ( error != kNoError ) {
		throw BECurl_Exception ( error );
	}	
	
	curl = curl_easy_init ();
	
	if ( !curl ) {
		throw bad_alloc(); // curl_easy_init thinks all errors are memory errors
	}
		
	if ( g_oauth ) {

		int oauth_error = g_oauth->sign_url ( url, parameters );
		if ( oauth_error != kNoError ) {
			throw BECurl_Exception ( CURLE_LOGIN_DENIED );
		}

	} else {
		set_username_and_password ( username, password );

		// curl doesn't make a copy of the post_args so we have to stop them being dealloced before they're used
		parameters = post_parameters;
	}
	
	set_parameters ( );
	
	easy_setopt ( CURLOPT_USERAGENT, USER_AGENT_STRING );
	easy_setopt ( CURLOPT_SSL_VERIFYPEER, 0L );
	easy_setopt ( CURLOPT_SSL_VERIFYHOST, 0L );
	
#if defined(FMX_MAC_TARGET)
	#warning experimental - stop fms running out of file descriptors under heavy usage ???
#endif
	easy_setopt ( CURLOPT_FORBID_REUSE, 1L );

}



BECurl::~BECurl()
{
	if ( curl ) { curl_easy_cleanup ( curl ); }
}


#pragma mark -
#pragma mark Methods
#pragma mark -


vector<char> BECurl::download ( )
{
	
	try {
		
		prepare ( );
		
		// save the data to file or memory?
		
		if ( filename.empty() ) {
			write_to_memory ( );		
		} else {
			
			FILE * outputFile = fopen ( filename.c_str(), "wb" );
			
			// curl will crash rather than fail with an error if outputFile is not open
			
			if ( outputFile ) {
				easy_setopt ( CURLOPT_WRITEDATA, outputFile );
			} else {
				throw BECurl_Exception ( CURLE_WRITE_ERROR );
			}
			
		}
		
		// download this
		easy_setopt ( CURLOPT_URL, url.c_str() );
		perform ( );
		
	} catch ( BECurl_Exception& e ) {
		error = e.code();
	}
	
	cleanup ();
	
	return result;
	
}	//	download



vector<char> BECurl::http_put ( )
{
	
	try {
		
		prepare ( );
		write_to_memory ( );		
		easy_setopt ( CURLOPT_UPLOAD, 1L );
		
		path path = filename;			
		
		// no directories etc.
		if ( exists ( path ) && is_regular_file ( path ) ) {
			
			upload_file = fopen ( filename.c_str(), "rb" );			
			easy_setopt ( CURLOPT_READDATA, upload_file );
			easy_setopt ( CURLOPT_INFILESIZE, (curl_off_t)file_size ( path ) );
			
			// HTTP PUT please
			easy_setopt ( CURLOPT_PUT, 1L );
			
			// put this
			easy_setopt ( CURLOPT_URL, url.c_str() );
			perform ( );
			
		} else {
			error = (CURLcode)kFileSystemError;
		}
		
	} catch ( filesystem_error& e ) {
		error = (CURLcode)e.code().value();
	} catch ( BECurl_Exception& e ) {
		error = e.code();
	}
	
	cleanup ();
	
	return result;
	
}	//	delete



vector<char> BECurl::http_delete ( )
{
	try {
		
		prepare ( );
		write_to_memory ( );		
		
		easy_setopt ( CURLOPT_CUSTOMREQUEST, "DELETE" );
		
		// delete this
		easy_setopt ( CURLOPT_URL, url.c_str() );
		perform ( );
		
		cleanup ();

	} catch ( BECurl_Exception& e ) {
		error = e.code();
	}
	
	return result;
	
}	//	delete



void BECurl::set_parameters ( )
{
	
	// add the parameters to the form
	if ( !parameters.empty() ) {
		easy_setopt ( CURLOPT_POSTFIELDS, parameters.c_str() );
		easy_setopt ( CURLOPT_POSTFIELDSIZE, parameters.length() );
	}
	
}	//	set_parameters


void BECurl::set_username_and_password ( const string username, const string password )
{
	
	// set user name and password for the authentication
	if ( !username.empty() ) {
		
		easy_setopt ( CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY );
		
		string username_and_password = username;
		
		if ( !password.empty() ) {
			username_and_password.append ( ":" );
			username_and_password.append ( password );
		}
		
		easy_setopt ( CURLOPT_USERPWD, username_and_password.c_str() );
		
	}
	
}	//	set_username_and_password



void BECurl::set_proxy ( struct host_details proxy_server )
{
	proxy = proxy_server.host;
	if ( !proxy_server.port.empty() ) {
		proxy += ":" + proxy_server.port;
	}
	
	proxy_login = proxy_server.username;
	if ( !proxy_server.password.empty() ) {
		proxy_login += ":" + proxy_server.password;
	}
	
}	//	set_proxy



void BECurl::set_options ( BECurlOptionMap options )
{
	
	BECurlOptionMap::iterator it = options.begin();
	while ( it != options.end() ) {
		
		boost::shared_ptr<BECurlOption>  curl_option = it->second;
		
		switch ( curl_option->type() ) {
				
			case BECurlOption::type_string:
				easy_setopt ( curl_option->option(), curl_option->as_string().c_str() );
				break;
				
			case BECurlOption::type_long:
				easy_setopt ( curl_option->option(), curl_option->as_long() );
				break;
				
			case BECurlOption::type_curl_off_t:
				easy_setopt ( curl_option->option(), curl_option->as_curl_off_t() );
				break;
				
			default:
				break;
		}
		
		++it;
	}
	
	
}	//	set_options



#pragma mark -
#pragma mark Protected Methods
#pragma mark -


void BECurl::prepare ( )
{
	
	if ( !proxy.empty() ) {
		easy_setopt ( CURLOPT_PROXY, proxy.c_str() );
		easy_setopt ( CURLOPT_PROXYUSERPWD, proxy_login.c_str() );
		easy_setopt ( CURLOPT_PROXYAUTH, CURLAUTH_ANY );
	}

	add_custom_headers ( );
	
	// send all headers & data to these functions
	
	headers = InitalizeCallbackMemory();
	easy_setopt ( CURLOPT_WRITEHEADER, (void *)&headers );
	
	data = InitalizeCallbackMemory();
	easy_setopt ( CURLOPT_HEADERFUNCTION, WriteMemoryCallback );
	
	// any custom options
	set_options ( g_curl_options );
	
}	//	prepare


void BECurl::add_custom_headers ( )
{
	
	custom_headers = NULL;
	
	// add any custom headers
	CustomHeaders::const_iterator it = http_custom_headers.begin();
	while ( it != http_custom_headers.end() ) {
		string custom_header = it->first;
		custom_header.append ( ": " );
		custom_header.append ( it->second );
		custom_headers = curl_slist_append ( custom_headers, custom_header.c_str() );
		++it;
	}
	
	if ( custom_headers ) {
		easy_setopt ( CURLOPT_HTTPHEADER, custom_headers );
	}
	
}	//	add_custom_headers


void BECurl::write_to_memory ( )
{
	// send all data to this function
	easy_setopt ( CURLOPT_WRITEFUNCTION, WriteMemoryCallback );
	easy_setopt ( CURLOPT_WRITEDATA, (void *)&data );	
}


void BECurl::perform ( )
{
	
	error = curl_easy_perform ( curl );
	
	if ( error == kNoError ) {

		error = curl_easy_getinfo ( curl, CURLINFO_RESPONSE_CODE, &http_response_code );
		
		if ( error == kNoError ) {
			// record the header information
			http_response_headers.erase();
			for ( size_t i = 0 ; i < headers.size ; i++ ) {
				http_response_headers.push_back ( headers.memory[i] );
			}
		
			// record the download response
			result.reserve ( data.size );
			for ( size_t i = 0 ; i < data.size ; i++ ) {
				result.push_back ( data.memory[i] );
			}
		}
	} else {
		throw BECurl_Exception ( error );
	}
	
}	//	easy_perform


void BECurl::cleanup ( )
{
	
	if ( upload_file ) { fclose ( upload_file ); }
	
	be_free ( headers.memory );
	be_free ( data.memory );
	
	if ( custom_headers ) { curl_slist_free_all ( custom_headers ); }	
	
}



void BECurl::easy_setopt ( CURLoption option, ... )
{
	va_list curl_parameter;
	va_start ( curl_parameter, option );
	error = curl_easy_setopt ( curl, option, va_arg ( curl_parameter, void * ) );
	va_end ( curl_parameter );
	if ( error ) {
		throw BECurl_Exception ( error );
	}
	
}
