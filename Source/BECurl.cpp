/*
 BECurl.cpp
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BECurl.h"
#include "BEPluginGlobalDefines.h"


#include <map>
#include <errno.h>



#pragma mark -
#pragma mark Globals
#pragma mark -

int g_http_response_code;
string g_http_response_headers;

CustomHeaders g_http_custom_headers;


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
	
	data.memory = (char *)malloc(1);  // this is grown as needed by WriteMemoryCallback 
	data.memory[0] = '\0'; 
	data.size = 0;
	
	return data;
	
}


static vector<char> PerformAction ( BECurl * curl )
{	
	curl->set_custom_headers ( g_http_custom_headers );
	vector<char>data = curl->download();
	g_http_response_code = curl->response_code();
	g_http_response_headers = curl->response_headers();
	g_http_custom_headers.clear();
	
	return data;
	
} // Perform



vector<char> GetURL ( const string url, const string filename, const string username, const string password )
{	
	BECurl * curl = new BECurl ( url, filename, username, password );
	return PerformAction ( curl );		
}


vector<char> HTTP_POST ( const string url, const string parameters )
{	
	BECurl * curl = new BECurl ( url, parameters );
	return PerformAction ( curl );		
}



#pragma mark -
#pragma mark Constructors
#pragma mark -

BECurl::BECurl ( const string download_this, const string to_file, const string username, const string password )
{
	init ( download_this, to_file, username, password, "" );
}


BECurl::BECurl ( const string download_this, const string post_parameters )
{
	init ( download_this, "", "", "", post_parameters );
}



void BECurl::init ( const string download_this, const string to_file, const string username, const string password, const string post_parameters )
{
	url = download_this;
	filename = to_file;
	
	http_response_code = 0;
	
	// set up curl as much as we can
	
	g_last_error = curl_global_init ( CURL_GLOBAL_ALL );
	if ( g_last_error != kNoError ) {
		throw BECurl_Exception();
	}	
	
	curl = curl_easy_init ();
	
	if ( !curl ) {
		g_last_error = kLowMemoryError; // curl_easy_init thinks all errors are memory errors
		throw bad_alloc();
	}
	
	set_username_and_password ( username, password );

	// curl doesn't make a copy of the post_args so we have to stop them being dealloced before they're used
	parameters = post_parameters;
	set_parameters ( );
	
	curl_easy_setopt ( curl, CURLOPT_USERAGENT, USER_AGENT_STRING );		
	curl_easy_setopt ( curl, CURLOPT_SSL_VERIFYPEER, 0L );
	curl_easy_setopt ( curl, CURLOPT_SSL_VERIFYHOST, 0L );
	
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
	
	prepare ( );
	
	try {
		
		// save the data to file or memory?
		
		if ( filename.empty() ) {
			write_to_memory ( );		
		} else {
			
			FILE *outputFile = fopen ( filename.c_str(), "wb" );
			
			// curl will crash rather than fail with an error if outputFile is not open
			
			if ( outputFile ) {
				curl_easy_setopt ( curl, CURLOPT_WRITEDATA, outputFile );
			} else {
				g_last_error = errno;
				throw BECurl_Exception();
			}				
		}
		
		// download this
		curl_easy_setopt ( curl, CURLOPT_URL, url.c_str() );
		perform ( );
		
	} catch ( BECurl_Exception e ) {
		; // nothing to to ... g_last_error set above
	}
	
	cleanup ();
	
	return result;
	
}	//	download



void BECurl::set_parameters ( )
{

	// add the parameters to the form
	if ( !parameters.empty() ) {
		curl_easy_setopt ( curl, CURLOPT_POSTFIELDS, parameters.c_str() );
		curl_easy_setopt ( curl, CURLOPT_POSTFIELDSIZE, parameters.length() );
	}

}	//	set_parameters


void BECurl::set_username_and_password ( const string username, const string password )
{
	
	// set user name and password for the authentication
	if ( !( username.empty() || password.empty() ) ) {
		
		curl_easy_setopt ( curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY );
		
		string username_and_password = username;
		username_and_password.append ( ":" );
		username_and_password.append ( password );
		
		curl_easy_setopt ( curl, CURLOPT_USERPWD, username_and_password.c_str() );
		
	}
	
}	//	set_username_and_password



#pragma mark -
#pragma mark Protected Methods
#pragma mark -


void BECurl::prepare ( )
{
	
	add_custom_headers ( );
	
	// send all headers & data to these functions
	
	headers = InitalizeCallbackMemory();
	curl_easy_setopt ( curl, CURLOPT_WRITEHEADER, (void *)&headers );
	
	data = InitalizeCallbackMemory();
	curl_easy_setopt ( curl, CURLOPT_HEADERFUNCTION, WriteMemoryCallback );
	
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
		curl_easy_setopt ( curl, CURLOPT_HTTPHEADER, custom_headers );
	}
	
}	//	add_custom_headers


void BECurl::write_to_memory ( )
{
	// send all data to this function
	curl_easy_setopt ( curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback );
	curl_easy_setopt ( curl, CURLOPT_WRITEDATA, (void *)&data );	
}

void BECurl::perform ( )
{
	
	g_last_error = curl_easy_perform ( curl );
	
	if ( g_last_error == kNoError ) {
		// get response code
		g_last_error = curl_easy_getinfo ( curl, CURLINFO_RESPONSE_CODE, &http_response_code );
		
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
	
}	//	easy_perform


void BECurl::cleanup ( )
{
	
	if ( headers.memory ) { free ( headers.memory ); }
	if ( data.memory ) { free ( data.memory ); }
	
	if ( custom_headers ) { curl_slist_free_all ( custom_headers ); }	
	
}
