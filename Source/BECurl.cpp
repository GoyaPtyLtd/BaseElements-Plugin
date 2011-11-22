/*
 BECurl.cpp
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BECurl.h"
#include "BEPluginGlobalDefines.h"


#if defined(FMX_WIN_TARGET)

	#include "windows.h"

	#define PATH_MAX MAX_PATH

#endif

#include "curl/curl.h"

#include <map>


#pragma mark -
#pragma mark Globals
#pragma mark -

int g_http_response_code;
string g_http_response_headers;

CustomHeaders g_http_custom_headers;


#pragma mark -
#pragma mark Functions
#pragma mark -


struct MemoryStruct {
	char *memory;
	size_t size;
};


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


static vector<char> Perform ( BECurl * curl )
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
	return Perform ( curl );		
}


vector<char> HTTP_POST ( const StringAutoPtr url, const StringAutoPtr parameters )
{	
	BECurl * curl = new BECurl ( *url, *parameters );
	return Perform ( curl );		
}



#pragma mark -
#pragma mark Constructors
#pragma mark -

BECurl::BECurl ( string download_this, string to_file, string user, string pass )
{
	url = download_this;
	filename = to_file;
	username = user;
	password = pass;

	http_response_code = 0;
}


BECurl::BECurl ( string download_this, string with )
{
	url = download_this;
	parameters = with;

	http_response_code = 0;
}



BECurl::~BECurl()
{
}


#pragma mark -
#pragma mark Methods
#pragma mark -

vector<char> BECurl::download ( )
{
	CURL *curl;
	vector<char> result;
	
	g_last_error = curl_global_init ( CURL_GLOBAL_ALL );
	if ( g_last_error != kNoError ) {
		return result;
	}
	
	
	curl = curl_easy_init ();
	
	if ( curl ) {
				
		struct MemoryStruct headers = InitalizeCallbackMemory();
		struct MemoryStruct data = InitalizeCallbackMemory();
		
		// add the parameters to the form
		if ( !parameters.empty() ) {
			curl_easy_setopt ( curl, CURLOPT_POSTFIELDS, parameters.c_str() );
			curl_easy_setopt ( curl, CURLOPT_POSTFIELDSIZE, parameters.length() );
		}
		
		// set user name and password for the authentication
		if ( !( username.empty() || password.empty() ) ) {
			
			curl_easy_setopt ( curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY );
			
			string username_and_password = username;
			username_and_password.append ( ":" );
			username_and_password.append ( password );
			
			curl_easy_setopt ( curl, CURLOPT_USERPWD, username_and_password.c_str() );
			
		}
		
		curl_easy_setopt ( curl, CURLOPT_USERAGENT, USER_AGENT_STRING );		
		curl_easy_setopt ( curl, CURLOPT_SSL_VERIFYPEER, 0L );
		curl_easy_setopt ( curl, CURLOPT_SSL_VERIFYHOST, 0L );
		
		// send all header data to this function
		curl_easy_setopt ( curl, CURLOPT_HEADERFUNCTION, WriteMemoryCallback );
		curl_easy_setopt ( curl, CURLOPT_WRITEHEADER, (void *)&headers );
		
		// save the data to file or memory?
		FILE *outputFile;
		if ( filename.empty() ) {
			// send all data to this function
			curl_easy_setopt ( curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback );
			curl_easy_setopt ( curl, CURLOPT_WRITEDATA, (void *)&data );		
		} else {
			outputFile = fopen ( filename.c_str(), "wb" );
			curl_easy_setopt ( curl, CURLOPT_WRITEDATA, outputFile );
		}
		
		
		// add any custom headers
		struct curl_slist *custom_headers = NULL;	
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
		
		
		// download this
		curl_easy_setopt ( curl, CURLOPT_URL, url.c_str() );
		
		g_last_error = curl_easy_perform ( curl );
		
		if ( g_last_error == kNoError ) {
			// get response code
			g_last_error = curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &http_response_code );
			
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
		
		// cleanup
		curl_easy_cleanup ( curl );
		curl_slist_free_all ( custom_headers );

		if ( headers.memory ) { free ( headers.memory ); }
		if ( data.memory ) { free ( data.memory ); }
		
	}
	
	return result;
		
}	//	download

