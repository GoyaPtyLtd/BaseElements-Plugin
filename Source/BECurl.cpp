/*
 BECurl.cpp
 BaseElements Plug-In
 
 Copyright 2011-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEPluginGlobalDefines.h"
#include "BECurl.h"
#include "BEOAuth.h"
#include "BEFileMakerPlugin.h"

#include "BEPluginUtilities.h"

#if defined ( FMX_WIN_TARGET )
	#include "BEWinFunctions.h"
	#include <tchar.h>
#endif

#if defined ( FMX_MAC_TARGET )
	#include "BEMacFunctions.h"
#endif

#include <iostream>
#include <sstream>
#include <errno.h>

#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/scoped_ptr.hpp>


using namespace std;
using namespace boost::filesystem;


#pragma mark -
#pragma mark Globals
#pragma mark -

int g_http_response_code;
string g_http_response_headers;
CustomHeaders g_http_custom_headers;
struct host_details g_http_proxy;
BECurlOptionMap g_curl_options;

extern BEOAuth * g_oauth;
extern BEFileMakerPlugin * g_be_plugin;


#pragma mark -
#pragma mark Functions
#pragma mark -


size_t WriteMemoryCallback (void *ptr, size_t size, size_t nmemb, void *data );
int SeekFunction ( void *instream, curl_off_t offset, int origin );
MemoryStruct InitalizeCallbackMemory ( void );
int progress_dialog ( void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow );
int old_progress_dialog ( void *p, double dltotal, double dlnow, double ultotal, double ulnow );


#pragma mark -
#pragma mark Callbacks
#pragma mark -


size_t WriteMemoryCallback (void *ptr, size_t size, size_t nmemb, void *data )
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


size_t ReadMemoryCallback (void *ptr, size_t size, size_t nmemb, void *data )
{
	
	size_t curl_size = nmemb * size;
	
	struct MemoryStruct * userdata = (struct MemoryStruct *)data;
	
	size_t to_copy = ( userdata->size < curl_size ) ? userdata->size : curl_size;
	memcpy ( ptr, userdata->memory, to_copy );
	userdata->origin = userdata->memory;
	userdata->size -= to_copy;
	userdata->memory += to_copy;
	
	return to_copy;
	
}


// required when doing authenticated http put

int SeekFunction ( void *instream, curl_off_t offset, int origin )
{
	int result = CURL_SEEKFUNC_OK;
	
	struct MemoryStruct *userdata = (struct MemoryStruct *)instream;
	
	if ( origin == SEEK_SET ) {
		userdata->size = (size_t)userdata->memory - (size_t)userdata->origin + (size_t)offset;
		userdata->memory = userdata->origin + offset;
	} else {
		// shouldn't be here
		result = CURL_SEEKFUNC_CANTSEEK;
	}
	
	return result;
}


MemoryStruct InitalizeCallbackMemory ( void )
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


#pragma mark -
#pragma mark Dialogs
#pragma mark -


int progress_dialog ( void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow )
{
#pragma unused ( p )
	
	const curl_off_t total_bytes = dltotal > 0 ? dltotal : ultotal;
	curl_off_t bytes_so_far = dlnow > 0 ? dlnow : ulnow;
	const wstring direction = dltotal > 0 ? L"Down" : L"Up";
	
	const double bytes_per_megabyte = 1000000;
	const double mb_total = (double)total_bytes / bytes_per_megabyte;
	const double mb_so_far = (double)bytes_so_far / bytes_per_megabyte;
	
	WStringAutoPtr description ( new wstring );
	
	fmx::errcode error = kNoError;
	static boost::posix_time::ptime last;
	
	const bool completed = dlnow == dltotal;
	
	static bool visible = false;
	
	if ( visible ) {
		
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		const boost::posix_time::time_duration msdiff = now - last;
		
		// only update 5 times a second... slows down too much otherwise
		
		const boost::posix_time::hours::tick_type how_long = msdiff.total_milliseconds();
		
		if ( how_long > 200 || completed ) {
			
			wstringstream d;
			d.precision ( 1 );
			wstring mb_suffix = L" MB";
			d << direction << L"loading... " << fixed << mb_so_far << mb_suffix << L" of " << fixed << mb_total << mb_suffix;
			description->assign ( d.str() );
			
			if ( completed ) {
				++bytes_so_far;
			}
			
			error = UpdateProgressDialog ( (unsigned long)bytes_so_far, description );
			
			last = now;
		}
	} else {
		
		if ( ! completed ) {
			
			WStringAutoPtr title ( new wstring ( direction + L"load Progress" ) );
			description->append ( L"Starting " + direction + L"load." );
			
			error = DisplayProgressDialog ( title, description, (unsigned long)total_bytes, false /* AllowUserAbort ( *environment ) */ );
			
		}
	}
	
	visible = completed ? false : error == CURLE_OK;
	
	g_last_error = error;
	
	return error;
}


/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
int old_progress_dialog ( void *p, double dltotal, double dlnow, double ultotal, double ulnow )
{
	int error = kNoError;
	
	if ( dltotal > 0 || ultotal > 0 ) {
		error = progress_dialog ( p, (curl_off_t)dltotal, (curl_off_t)dlnow, (curl_off_t)ultotal, (curl_off_t)ulnow );
	}
	
	return error;
}



#pragma mark -
#pragma mark Constructors
#pragma mark -


BECurl::BECurl ( )
{
	Init ();
}


BECurl::BECurl ( const string download_this, const be_http_method method, const boost::filesystem::path to_file, const string _username, const string _password, const string post_parameters, const vector<char> put_data )
{

	Init ();
	
	url = download_this;
	http_method = method;
	filename = to_file;
	upload_data = put_data;
	username = _username;
	password = _password;
	parameters = post_parameters;
	
	if ( g_oauth ) {
		
		int oauth_error = g_oauth->sign_url ( url, parameters, http_method_as_string() );
		if ( oauth_error != kNoError ) {
			throw BECurl_Exception ( CURLE_LOGIN_DENIED );
		}
	
	} else {
		set_username_and_password ( );
	}

	// send all headers & data to these functions
	
	headers = InitalizeCallbackMemory();
	easy_setopt ( CURLOPT_WRITEHEADER, (void *)&headers );
	
	data = InitalizeCallbackMemory();
	easy_setopt ( CURLOPT_HEADERFUNCTION, WriteMemoryCallback );
	
	// any custom options, headers etc.
	
	set_parameters ( );
	set_proxy ( g_http_proxy );
	set_custom_headers ( g_http_custom_headers );
	
	configure_progress_dialog ( );
	
	easy_setopt ( CURLOPT_USERAGENT, USER_AGENT_STRING );
	easy_setopt ( CURLOPT_SSL_VERIFYPEER, 0L );
	easy_setopt ( CURLOPT_SSL_VERIFYHOST, 0L );
	easy_setopt ( CURLOPT_FORBID_REUSE, 1L ); // stop fms running out of file descriptors under heavy usage
	
	// allow the user to override anything we set
	set_options ( g_curl_options );
	
}


BECurl::~BECurl()
{
	curl_easy_cleanup ( curl );
	curl_formfree ( post_data );
	curl_global_cleanup();
}


#pragma mark -
#pragma mark Methods
#pragma mark -



void BECurl::Init ( )
{
	// must intialise, we crash otherwise
	upload_file = NULL;
	custom_headers = NULL;
	post_data = NULL;
	
	//
	http_response_code = 0;
	
	// set up curl as much as we can
	
	error = curl_global_init ( CURL_GLOBAL_ALL );
	if ( error != kNoError ) {
		throw BECurl_Exception ( error );
	}
	
	curl = curl_easy_init (); // must be the first libcurl function called (afer curl_global_init)
	if ( !curl ) {
		throw bad_alloc(); // curl_easy_init thinks all errors are memory errors
	}
	
}



vector<char> BECurl::perform_action ( )
{
	vector<char>response;
	
	switch ( get_http_method() ) {
			
		case kBE_FTP_METHOD_UPLOAD:
			response = ftp_upload ( );
			break;
			
		case kBE_HTTP_METHOD_DELETE:
			response = http_delete ( );
			break;
			
		case kBE_HTTP_METHOD_PUT:
			response = http_put ( );
			break;
			
		default:
			response = download ( );
			break;
	}
	
	cleanup ();
	
	g_last_error = last_error();
	g_http_response_code = response_code();
	g_http_response_headers = response_headers();
	g_http_custom_headers.clear();
	
	return response;
	
} // perform_action



vector<char> BECurl::download ( )
{
	
	try {
				
		// save the data to file or memory?
		
		if ( filename.empty() ) {
			write_to_memory ( );		
		} else {
			
			upload_file = FOPEN ( filename.c_str(), _TEXT ( "wb" ) );
			
			// curl will crash rather than fail with an error if outputFile is not open
			
			if ( upload_file ) {
				easy_setopt ( CURLOPT_WRITEDATA, upload_file );
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
	
	return result;
	
}	//	download



vector<char> BECurl::http_put ( )
{
	
	try {
		
		prepare_upload ( );
		
		if ( filename.empty() ) {
			
			prepare_data_upload ( );

		} else {
			
			// no directories etc.
			if ( exists ( filename ) && is_regular_file ( filename ) ) {
				
				upload_file = FOPEN ( filename.c_str(), _TEXT ( "rb" ) );

				easy_setopt ( CURLOPT_READDATA, upload_file );
				easy_setopt ( CURLOPT_INFILESIZE, (curl_off_t)file_size ( filename ) );
				
			} else {
				throw BECurl_Exception ( (CURLcode)kFileExistsError );
			}

		}
		
		easy_setopt ( CURLOPT_PUT, 1L );
		
		// put this
		easy_setopt ( CURLOPT_URL, url.c_str() );
		
		perform ( );
				
	} catch ( filesystem_error& e ) {
		error = (CURLcode)e.code().value();
	} catch ( BECurl_Exception& e ) {
		error = e.code();
	}
	
	return result;
	
}	//	http_put



vector<char> BECurl::http_delete ( )
{
	try {
		
		write_to_memory ( );
		
		easy_setopt ( CURLOPT_CUSTOMREQUEST, HTTP_METHOD_DELETE );
		
		// delete this
		easy_setopt ( CURLOPT_URL, url.c_str() );
		perform ( );
		
	} catch ( BECurl_Exception& e ) {
		error = e.code();
	}
	
	return result;
	
}	//	delete



vector<char> BECurl::ftp_upload ( )
{
	
	try {
		
		prepare_upload ( );
		prepare_data_upload ( );
				
		// upload this
		easy_setopt ( CURLOPT_URL, url.c_str() );
		
		perform ( );
		
	} catch ( filesystem_error& e ) {
		error = (CURLcode)e.code().value();
	} catch ( BECurl_Exception& e ) {
		error = e.code();
	}
	
	return result;
	
}	//	ftp_upload



void BECurl::set_parameters ( )
{

	if ( !parameters.empty() ) {
		
		vector<string> fields;
		boost::split ( fields, parameters, boost::is_any_of ( "&" ) );
		 
		struct curl_httppost *last_form_field = NULL;

		for ( vector<string>::iterator it = fields.begin() ; it != fields.end(); ++it ) {
			
			vector<string> key_value_pair;
			boost::split ( key_value_pair, *it, boost::is_any_of ( "=" ) );
				
			// get rid of @ sign that marks that it's a file path
			int value_type = CURLFORM_COPYCONTENTS;
			string value = key_value_pair[1];
			if ( !value.empty() && value[0] == '@' ) {
				value.erase ( value.begin() );
				value_type = CURLFORM_FILE;
			}
				
			// add the field
			curl_formadd ( &post_data, &last_form_field, CURLFORM_COPYNAME, key_value_pair[0].c_str(), value_type, value.c_str(), CURLFORM_END );
	
		} // for
		
		easy_setopt ( CURLOPT_HTTPPOST, post_data );

	} // if ( !parameters.empty() )
	
}	//	set_parameters



void BECurl::set_custom_headers ( CustomHeaders http_custom_headers )
{

	if ( custom_headers ) {
		curl_slist_free_all ( custom_headers );
		custom_headers = NULL;
	}

	// add any custom headers
	CustomHeaders::const_iterator it = http_custom_headers.begin();
	while ( it != http_custom_headers.end() ) {
		string custom_header = join ( it->first, it->second, ": " );
		custom_headers = curl_slist_append ( custom_headers, custom_header.c_str() );
		++it;
	}
	
	if ( custom_headers ) {
		easy_setopt ( CURLOPT_HTTPHEADER, custom_headers );
	}

}



void BECurl::set_username_and_password ( )
{
	
	// set user name and password for the authentication
	if ( !username.empty() ) {
		
		string username_and_password = join ( username, password );
		easy_setopt ( CURLOPT_USERPWD, username_and_password.c_str() );
		easy_setopt ( CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY );
		
	}
	
}	//	set_username_and_password



void BECurl::set_proxy ( struct host_details proxy_server )
{

	string proxy = join ( proxy_server.host, proxy_server.port );

	if ( !proxy.empty() ) {
		
		string proxy_login = join ( proxy_server.username, proxy_server.password );
		
		easy_setopt ( CURLOPT_PROXY, proxy.c_str() );
		easy_setopt ( CURLOPT_PROXYUSERPWD, proxy_login.c_str() );
		easy_setopt ( CURLOPT_PROXYAUTH, CURLAUTH_ANY );
	
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
			case BECurlOption::type_named_constant:
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
	
	if ( g_be_plugin->running_on_server ( ) ) {
		easy_setopt ( CURLOPT_NOPROGRESS, 1L );
	}
	
}	//	set_options



#pragma mark -
#pragma mark Protected Methods
#pragma mark -


void BECurl::write_to_memory ( )
{
	// send all data to this function
	easy_setopt ( CURLOPT_WRITEFUNCTION, WriteMemoryCallback );
	easy_setopt ( CURLOPT_WRITEDATA, (void *)&data );	
}


void BECurl::perform ( )
{
	error = curl_easy_perform ( curl );
	
	if ( error == CURLE_OK ) {

		error = curl_easy_getinfo ( curl, CURLINFO_RESPONSE_CODE, &http_response_code );
		
		if ( error == CURLE_OK ) {
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
		} else {
			throw BECurl_Exception ( error );
		}
		
	} else {
		throw BECurl_Exception ( error );
	}
	
}	//	easy_perform


void BECurl::cleanup ( )
{
	
	if ( upload_file ) {
		fclose ( upload_file );
		upload_file = NULL;
	}
	
	be_free ( headers.memory );
	be_free ( data.memory );
	
	if ( custom_headers ) {
		curl_slist_free_all ( custom_headers );
	}
	
} // cleanup


void BECurl::easy_setopt ( CURLoption option, ... )
{
	va_list curl_parameter;
	va_start ( curl_parameter, option );
	error = curl_easy_setopt ( curl, option, va_arg ( curl_parameter, void * ) );
	va_end ( curl_parameter );
	if ( error ) {
		throw BECurl_Exception ( error );
	}
	
} // easy_setopt


void BECurl::configure_progress_dialog ( )
{
	// libcurl uses CURLOPT_XFERINFOFUNCTION if it can and CURLOPT_PROGRESSFUNCTION when it cannot
	
	error = curl_easy_setopt ( curl, CURLOPT_PROGRESSFUNCTION, old_progress_dialog );
	if ( error ) {
		throw BECurl_Exception ( error );
	}
	
	// xferinfo was introduced in 7.32.0
	
#if LIBCURL_VERSION_NUM >= 0x072000
	
	error = curl_easy_setopt ( curl, CURLOPT_XFERINFOFUNCTION, progress_dialog );
	if ( error != CURLE_OK && error != CURLE_UNKNOWN_OPTION ) {
		throw BECurl_Exception ( error );
	}
	
#endif
	
	// and disable it... enabled by the user when required
	easy_setopt ( CURLOPT_NOPROGRESS, 1L );
	
} // configure_progress_dialog


string BECurl::http_method_as_string ( )
{
	string method = "";
	
	switch ( http_method ) {
			
		case kBE_HTTP_METHOD_DELETE:
			method = HTTP_METHOD_DELETE;
			break;
			
		case kBE_HTTP_METHOD_GET:
			method = HTTP_METHOD_GET;
			break;
			
		case kBE_HTTP_METHOD_POST:
			method = HTTP_METHOD_POST;
			break;

		case kBE_HTTP_METHOD_PUT:
			method = HTTP_METHOD_PUT;
			break;
			
		default:
			break;
	}
	
	return method;
}


void BECurl::prepare_data_upload ( )
{
	
	easy_setopt ( CURLOPT_READFUNCTION, ReadMemoryCallback );
	
	userdata = InitalizeCallbackMemory ( );

	if ( ! upload_data.empty() ) {
		userdata.memory = &upload_data[0];
		userdata.size = upload_data.size();
	}

	easy_setopt ( CURLOPT_READDATA, &userdata );
	easy_setopt ( CURLOPT_INFILESIZE, userdata.size );
	
	easy_setopt ( CURLOPT_SEEKFUNCTION, SeekFunction );
	easy_setopt ( CURLOPT_SEEKDATA, &userdata );

}


void BECurl::prepare_upload ( )
{
	write_to_memory ( );
	easy_setopt ( CURLOPT_UPLOAD, 1L );
}
