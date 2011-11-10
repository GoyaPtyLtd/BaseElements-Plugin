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



// heavily based on http://curl.haxx.se/libcurl/c/getinmemory.html

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


vector<char> GetURL ( const StringAutoPtr url, const StringAutoPtr filename, const StringAutoPtr username, const StringAutoPtr password )
{	
	CURL *curl_handle;
	struct MemoryStruct chunk;
	
	chunk.memory = (char *)malloc(1);  // this is grown as needed by WriteMemoryCallback 
	chunk.memory[0] = '\0'; 
	chunk.size = 0;
	
	
	/* init the curl session */ 
	
	//	http://curl.haxx.se/libcurl/c/curl_global_init.html
	
	g_last_error = curl_global_init ( CURL_GLOBAL_ALL );
	curl_handle = curl_easy_init();
	
	// bail out now if initialisation fails
	if ( g_last_error != kNoError || curl_handle == NULL ) {
		vector<char> empty_string;
		return empty_string;
	}
	
	//	download this
	curl_easy_setopt ( curl_handle, CURLOPT_URL, url->c_str() );
	
	FILE *outputFile;
	if ( filename->length() != 0 ) {
		outputFile = fopen ( filename->c_str(), "wb" );
		curl_easy_setopt ( curl_handle, CURLOPT_WRITEDATA, outputFile );
	} else {
		// send all data to this function
		curl_easy_setopt ( curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback );
		curl_easy_setopt ( curl_handle, CURLOPT_WRITEDATA, (void *)&chunk );		
	}
	
	curl_easy_setopt ( curl_handle, CURLOPT_USERAGENT, USER_AGENT_STRING );		
	curl_easy_setopt ( curl_handle, CURLOPT_SSL_VERIFYPEER, 0L );
	curl_easy_setopt ( curl_handle, CURLOPT_SSL_VERIFYHOST, 0L );
	
	if ( !( username->empty() || password->empty() ) ) {
		
		curl_easy_setopt ( curl_handle, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY );
		
		string username_and_password = *username;
		username_and_password.append ( ":" );
		username_and_password.append ( *password );
		
		/* set user name and password for the authentication */ 
		curl_easy_setopt ( curl_handle, CURLOPT_USERPWD, username_and_password.c_str() );
		
	}
	
	g_last_error = curl_easy_perform ( curl_handle );
	curl_easy_cleanup ( curl_handle );
	
	vector<char> result;
	
	if ( filename->length() != 0 ) {
		fclose ( outputFile );
	} else {
		
		/// for the default allocator, bad_alloc is thrown if the allocation request does not succeed
		
		result.reserve ( chunk.size );
		
		for ( size_t i = 0 ; i < chunk.size ; i++ ) {
			result.push_back ( chunk.memory[i] );
		}
		
		//	clean up
		
		if ( chunk.memory ) {
			free ( chunk.memory );
		}
		
	}
	
	curl_global_cleanup ( );
	
	return result;
	
} // BE_GetURL


