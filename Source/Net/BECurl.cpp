/*
 BECurl.cpp
 BaseElements Plug-In

 Copyright 2011-2023 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEPluginGlobalDefines.h"
#include "BECppUtilities.h"
#include "BECurl.h"
#include "BEFileMakerPlugin.h"

#include "BEPluginUtilities.h"

#if defined FMX_MAC_TARGET

	#include "apple/BEMacFunctions.h"

#elif FMX_IOS_TARGET

	#include "apple/BEIOSFunctions.h"

#elif defined FMX_WIN_TARGET

	#include "win/BEWinFunctions.h"
	#include <tchar.h>

#elif defined FMX_LINUX_TARGET

	#include "linux/BELinuxFunctions.h"

#endif

#include <iostream>
#include <sstream>
#include <errno.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomma"
#include <boost/algorithm/string.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma clang diagnostic pop

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/scoped_ptr.hpp>
#pragma GCC diagnostic pop

#include <Poco/URI.h>
#include <Poco/Path.h>

using namespace std;
using namespace boost::filesystem;


#define FTP_DELETE_COMMAND "dele";
#define SFTP_DELETE_COMMAND "rm";
#define SFTP_SCHEME "sftp";


#pragma mark -
#pragma mark Globals
#pragma mark -

thread_local int g_http_response_code;
thread_local string g_http_response_headers;
thread_local std::stringstream g_curl_trace;
thread_local CustomHeaders g_http_custom_headers;
thread_local struct host_details g_http_proxy;
thread_local BECurlOptionMap g_curl_options;
thread_local std::map<std::string, std::string> g_curl_info;

extern BEFileMakerPlugin * g_be_plugin;


#pragma mark -
#pragma mark Functions
#pragma mark -


size_t WriteMemoryCallback (void *ptr, size_t size, size_t nmemb, void *data );
int SeekFunction ( void *instream, curl_off_t offset, int origin );
MemoryStruct InitalizeCallbackMemory ( void );
static void dump_trace_data ( const unsigned char *ptr, const size_t size );
static int trace_callback ( CURL * /* curl */, curl_infotype type, char * data, size_t size, void * /* userp */ );
int progress_dialog ( void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow );


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

	data.memory = NULL;  // allocated/grown as needed by WriteMemoryCallback
	data.size = 0;
	data.origin = NULL;

	return data;

}


// originally based on https://raw.githubusercontent.com/bagder/curl/master/docs/examples/debug.c

static void dump_trace_data ( const unsigned char *ptr, const size_t size )
{
	const unsigned long width = 0x40;

	g_curl_trace << ", " << size << " bytes (" << std::showbase << setw ( 2 ) << setfill ( '0' ) << std::hex << size << ")" << FILEMAKER_END_OF_LINE;

	for ( size_t i = 0; i < size; i += width ) {

		g_curl_trace  << setw ( 4 ) << setfill ( '0' )  << std::hex << i << ": ";

		for ( size_t c = 0; (c < width) && (i+c < size); c++ ) {

			// check for 0D0A; if found, skip past and start a new line
			if ( (i+c+1 < size) && ptr[i+c]==0x0D && ptr[i+c+1]==0x0A ) {
				i += c + 2 - width;
				break;
			}

			g_curl_trace << static_cast<char>((ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.');

			// check again for 0D0A, to avoid an extra \n if it's at width
			if ( (i+c+2 < size) && ptr[i+c+1]==0x0D && ptr[i+c+2]==0x0A ) {
				i += c + 3 - width;
				break;
			}

		}

		g_curl_trace << FILEMAKER_END_OF_LINE;

	}

}


static int trace_callback ( CURL * /* curl */, curl_infotype type, char * data, size_t size, void * /* userp */ )
{

	switch ( type ) {

		case CURLINFO_HEADER_OUT:
			g_curl_trace << "=> Send header";
			break;

		case CURLINFO_DATA_OUT:
			g_curl_trace << "=> Send data";
			break;

		case CURLINFO_SSL_DATA_OUT:
			g_curl_trace << "=> Send SSL data";
			break;

		case CURLINFO_HEADER_IN:
			g_curl_trace << "<= Recv header";
			break;

		case CURLINFO_DATA_IN:
			g_curl_trace << "<= Recv data";
			break;

		case CURLINFO_SSL_DATA_IN:
			g_curl_trace << "<= Recv SSL data";
			break;

		case CURLINFO_TEXT:
			g_curl_trace << "== Info: ";
			g_curl_trace << data;
			// falling through

		default: /* in case a new one is introduced to shock us */
			return kNoError;

	}

	dump_trace_data ( (unsigned char *)data, size );

	return kNoError;

} // trace_callback


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

	std::wstring description;

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
			description.assign ( d.str() );

			if ( completed ) {
				++bytes_so_far;
			}

			error = UpdateProgressDialog ( (unsigned long)bytes_so_far, description );

			last = now;
		}
	} else {

		if ( ! completed ) {

			auto title ( direction + L"load Progress" );
			description.append ( L"Starting " + direction + L"load." );

			error = DisplayProgressDialog ( title, description, (unsigned long)total_bytes, false /* AllowUserAbort ( *environment ) */ );

		}
	}

	visible = completed ? false : error == CURLE_OK;

	g_last_error = error;

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

	set_username_and_password();

	// send all headers & data to these functions

	easy_setopt ( CURLOPT_WRITEHEADER, (void *)&headers );
	easy_setopt ( CURLOPT_HEADERFUNCTION, WriteMemoryCallback );

	if ( http_method == kBE_HTTP_METHOD_POST ) {
		easy_setopt ( CURLOPT_POST, 1L );
	} else if ( http_method == kBE_HTTP_METHOD_PATCH ) {
		easy_setopt ( CURLOPT_CUSTOMREQUEST, HTTP_METHOD_PATCH );
	}

	// any custom options, headers etc.

	set_parameters ( );
	set_proxy ( g_http_proxy );
	set_custom_headers ( g_http_custom_headers );

	configure_progress_dialog ( );

	easy_setopt ( CURLOPT_USERAGENT, USER_AGENT_STRING );
	easy_setopt ( CURLOPT_FORBID_REUSE, 1L ); // stop fms running out of file descriptors under heavy usage

	// allow the user to override anything we set
	set_options ( g_curl_options );

}


BECurl::~BECurl()
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

	if ( command_list ) {
		curl_slist_free_all ( command_list );
	}

	g_last_error = error;
	g_http_response_code = http_response_code;
	g_http_response_headers = http_response_headers;
	
	g_http_custom_headers.clear();

	curl_mime_free ( mime );
	curl_easy_cleanup ( curl );
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
	command_list = NULL;
	mime = NULL;
	
	//
	http_response_code = 0;

	// set up curl as much as we can

	error = curl_global_init ( CURL_GLOBAL_ALL );
	if ( error != CURLE_OK ) {
		throw BECurl_Exception ( error );
	}

	curl = curl_easy_init (); // must be the first libcurl function called (afer curl_global_init)
	if ( !curl ) {
		throw bad_alloc(); // curl_easy_init thinks all errors are memory errors
	}

	easy_setopt ( CURLOPT_SSL_VERIFYPEER, 0L );
	easy_setopt ( CURLOPT_SSL_VERIFYHOST, 0L );

	// debug trace
	easy_setopt ( CURLOPT_DEBUGFUNCTION, trace_callback );
	//	easy_setopt ( CURLOPT_DEBUGDATA, &config );
	easy_setopt ( CURLOPT_VERBOSE, 1L ); // DEBUGFUNCTION has no effect unless enabled
	g_curl_trace.str ( "" );
	g_curl_trace.clear();

	headers = InitalizeCallbackMemory();
	data = InitalizeCallbackMemory();

}


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

	easy_setopt ( CURLOPT_PUT, 1L );

	return upload();

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
	return upload();
}	//	ftp_upload


vector<char> BECurl::ftp_delete ( )
{

	try {

		write_to_memory ( );

		// delete this
		const Poco::URI uri ( url );
		std::string path = uri.getPath();
		path.erase ( 0, 1 ); // lop off the leading slash

		// ftp and sftp use different commands
		std::string delete_command = FTP_DELETE_COMMAND;
		if ( boost::iequals ( uri.getScheme(), "sftp" ) ) { // SFTP_SCHEME
			delete_command = SFTP_DELETE_COMMAND;
		}

		const std::string ftp_command = delete_command + " " + path;
		command_list = curl_slist_append ( command_list, ftp_command.c_str() );
		easy_setopt ( CURLOPT_QUOTE, command_list );

		const std::string connect_to = uri.getScheme() + "://" + uri.getAuthority();
		easy_setopt ( CURLOPT_URL, connect_to.c_str() );

		perform ( );

	} catch ( BECurl_Exception& e ) {
		error = e.code();
	}

	return result;

}	//	ftp_delete



void BECurl::set_parameters ( )
{

	if ( http_method == kBE_HTTP_METHOD_POST || !parameters.empty() ) {

		if ( 0 == parameters.find ( "@" ) ) { // let curl do the work unless there's a file path

			parameters.erase ( parameters.begin() );

			// slurp up the file contents
			boost::filesystem::ifstream input_file ( parameters, ios_base::in | ios_base::binary | ios_base::ate );
			input_file.exceptions ( boost::filesystem::ofstream::badbit | boost::filesystem::ofstream::failbit );
			input_file.seekg ( 0, ios::beg );
			vector<char> file_data ( (std::istreambuf_iterator<char> ( input_file ) ), std::istreambuf_iterator<char>() );

			easy_setopt ( CURLOPT_POSTFIELDS, file_data.data() );
			easy_setopt ( CURLOPT_POSTFIELDSIZE, file_data.size() );

		} else if ( std::string::npos == parameters.find ( "=@" ) ) { // let curl do the work unless there's a file path

			easy_setopt ( CURLOPT_POSTFIELDS, parameters.c_str() );
			easy_setopt ( CURLOPT_POSTFIELDSIZE, parameters.length() );

		} else {

			vector<string> fields;
			boost::split ( fields, parameters, boost::is_any_of ( "&" ) );

			// Build an HTTP form
			mime = curl_mime_init ( curl );

			for ( vector<string>::iterator it = fields.begin() ; it != fields.end(); ++it ) {

				vector<string> key_value_pair;
				boost::split ( key_value_pair, *it, boost::is_any_of ( "=" ) );
				if ( 1 == key_value_pair.size() ) {
					key_value_pair.push_back ( "" );
				} else if ( 0 == key_value_pair.size() ) {
					break;
				}

				curl_mimepart * part = curl_mime_addpart ( mime );

				// get rid of @ sign that marks that it's a file path
				string value = key_value_pair.at ( 1 );
				if ( !value.empty() && value[0] == '@' ) {
					value.erase ( value.begin() );
					curl_mime_filedata ( part, value.c_str() );
				} else {
					curl_mime_data ( part, value.c_str(), CURL_ZERO_TERMINATED );
				}
				
				curl_mime_name ( part, key_value_pair.at(0).c_str() );

			} // for

			easy_setopt ( CURLOPT_MIMEPOST, mime );

		} // if ( std::string::npos == parameters.find ( "=@" ) )fi

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
	if ( ! username.empty() ) {

		easy_setopt ( CURLOPT_USERNAME, username.c_str() );
		easy_setopt ( CURLOPT_PASSWORD, password.c_str() );
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

		auto curl_option = it->second;

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


void BECurl::prepare_file_upload ( )
{

	// no directories etc.
	if ( exists ( filename ) && is_regular_file ( filename ) ) {

		upload_file = FOPEN ( filename.c_str(), _TEXT ( "rb" ) );

		easy_setopt ( CURLOPT_READDATA, upload_file );
		easy_setopt ( CURLOPT_INFILESIZE, (curl_off_t)file_size ( filename ) );

	} else {
		throw BECurl_Exception ( (CURLcode)kFileExistsError );
	}

}	//	file_upload


void BECurl::perform ( )
{
	error = curl_easy_perform ( curl );

	if ( error == CURLE_OK ) {

		get_curl_info();
		
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


void BECurl::easy_setopt ( const CURLoption option, ... )
{
	va_list curl_parameter;
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvarargs"
		va_start ( curl_parameter, option );
	#pragma GCC diagnostic pop

	error = curl_easy_setopt ( curl, option, va_arg ( curl_parameter, void * ) );
	va_end ( curl_parameter );
	if ( error ) {
		throw BECurl_Exception ( error );
	}

} // easy_setopt


void BECurl::configure_progress_dialog ( )
{

	error = curl_easy_setopt ( curl, CURLOPT_XFERINFOFUNCTION, progress_dialog );
	if ( error != CURLE_OK && error != CURLE_UNKNOWN_OPTION ) {
		throw BECurl_Exception ( error );
	}

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

		case kBE_HTTP_METHOD_PATCH:
			method = HTTP_METHOD_PATCH;
			break;

		default:
			break;
	}

	return method;
}


void BECurl::prepare_data_upload ( )
{

	if ( filename.empty() ) { // from memory

		easy_setopt ( CURLOPT_READFUNCTION, ReadMemoryCallback );

		userdata = InitalizeCallbackMemory();
		userdata.memory = upload_data.data();
		userdata.size = upload_data.size();

		easy_setopt ( CURLOPT_READDATA, &userdata );
		easy_setopt ( CURLOPT_INFILESIZE, userdata.size );

		easy_setopt ( CURLOPT_SEEKFUNCTION, SeekFunction );
		easy_setopt ( CURLOPT_SEEKDATA, &userdata );

	} else { // a file
		
		upload_file = fopen ( filename.string().c_str(), "rb" );
		easy_setopt ( CURLOPT_READDATA, upload_file );
		
	}

}


void BECurl::prepare_upload ( )
{
	write_to_memory ( );
	easy_setopt ( CURLOPT_UPLOAD, 1L );
	easy_setopt ( CURLOPT_URL, url.c_str() );
}


vector<char> BECurl::upload ( )
{

	try {

		prepare_upload ( );

		if ( filename.empty() ) {
			prepare_data_upload ( );
		} else {
			prepare_file_upload();
		}

		perform ( );

	} catch ( filesystem_error& e ) {
		error = (CURLcode)e.code().value();
	} catch ( BECurl_Exception& e ) {
		error = e.code();
	}

	return result;

}	// upload


const std::string BECurl::easy_getinfo ( const CURLINFO curl_info_option, const bool release_memory )
{

	char * getinfo_string;
	error = curl_easy_getinfo ( curl, curl_info_option, &getinfo_string );
	
	std::string the_info;
	
	if ( CURLE_OK == error ) {

		if ( getinfo_string ) {
			the_info = getinfo_string;
		} else {
		
			if ( release_memory ) {
				be_free ( getinfo_string );
			}
		
		}
	
	}
	
	return the_info;

}	// easy_getinfo


const std::string BECurl::easy_getinfo_long ( const CURLINFO curl_info_option )
{

	long getinfo_long;
	error = curl_easy_getinfo ( curl, curl_info_option, &getinfo_long );
	std::string the_info;
	
	if ( CURLE_OK == error ) {
		the_info = std::to_string ( getinfo_long );
	}
	
	return the_info;

}	// easy_getinfo_long


const std::string BECurl::easy_getinfo_double ( const CURLINFO curl_info_option )
{

	double getinfo_double;
	error = curl_easy_getinfo ( curl, curl_info_option, &getinfo_double );
	
	std::string the_info;
	
	if ( CURLE_OK == error ) {

		if ( 0 == getinfo_double ) { // we don't want 0.00000 etc.
			the_info = "0";
		} else {
			the_info = std::to_string ( getinfo_double );
		}
	
	}
	
	return the_info;

}	// easy_getinfo_double


const std::string BECurl::easy_getinfo_sizet ( const CURLINFO curl_info_option )
{

	curl_off_t getinfo_sizet;
	error = curl_easy_getinfo ( curl, curl_info_option, &getinfo_sizet );
	
	std::string the_info;
	
	if ( CURLE_OK == error ) {
		the_info = std::to_string ( getinfo_sizet );
	}
	
	return the_info;

}	// easy_getinfo_sizet


const std::string BECurl::easy_getinfo_cert ( const CURLINFO curl_info_option )
{

	struct curl_certinfo * cert_info;
	error = curl_easy_getinfo ( curl, curl_info_option, &cert_info );

	std::string the_info;

	if ( CURLE_OK == error ) {

		for ( auto i = 0 ; i < cert_info->num_of_certs ; i++ ) {

			struct curl_slist *slist;

			for ( slist = cert_info->certinfo[i]; slist; slist = slist->next ) {
				the_info.append ( slist->data );
				the_info.append ( "\n" );
			}
	
		}
		
	}

	return the_info;

}	// easy_getinfo_cert


const std::string BECurl::easy_getinfo_slist ( const CURLINFO curl_info_option )
{

	struct curl_slist * s_list;
	error = curl_easy_getinfo ( curl, curl_info_option, &s_list );

	std::string the_info;
	
	if ( CURLE_OK == error ) {

		auto next_list = s_list;

		while ( next_list ) {
			the_info.append ( next_list->data );
			the_info.append ( "\n" );
			next_list = next_list->next;
		}

		curl_slist_free_all ( s_list );
	
	}
	
	return the_info;

}	// easy_getinfo_slist


void BECurl::get_curl_info ( )
{

	// start afresh
	g_curl_info.clear();

	//		https://curl.se/libcurl/c/curl_easy_getinfo.html

	g_curl_info [ "CURLINFO_EFFECTIVE_METHOD" ] = easy_getinfo ( CURLINFO_EFFECTIVE_METHOD );
	g_curl_info [ "CURLINFO_EFFECTIVE_URL" ] = easy_getinfo ( CURLINFO_EFFECTIVE_URL );

	g_curl_info [ "CURLINFO_RESPONSE_CODE" ] = easy_getinfo_long ( CURLINFO_RESPONSE_CODE );
	// backwards compatibilty
	http_response_code = std::stoi ( g_curl_info [ "CURLINFO_RESPONSE_CODE" ] );

	g_curl_info [ "CURLINFO_REFERER" ] = easy_getinfo ( CURLINFO_REFERER );
	g_curl_info [ "CURLINFO_HTTP_CONNECTCODE" ] = easy_getinfo_long ( CURLINFO_HTTP_CONNECTCODE );
	g_curl_info [ "CURLINFO_HTTP_VERSION" ] = easy_getinfo_long ( CURLINFO_HTTP_VERSION );
	g_curl_info [ "CURLINFO_FILETIME" ] = easy_getinfo_long ( CURLINFO_FILETIME );
	g_curl_info [ "CURLINFO_FILETIME_T" ] = easy_getinfo_sizet ( CURLINFO_FILETIME_T );
	g_curl_info [ "CURLINFO_TOTAL_TIME" ] = easy_getinfo_double ( CURLINFO_TOTAL_TIME );
	g_curl_info [ "CURLINFO_TOTAL_TIME_T" ] = easy_getinfo_sizet ( CURLINFO_TOTAL_TIME_T );
	g_curl_info [ "CURLINFO_NAMELOOKUP_TIME" ] = easy_getinfo_double ( CURLINFO_NAMELOOKUP_TIME );
	g_curl_info [ "CURLINFO_NAMELOOKUP_TIME_T" ] = easy_getinfo_sizet ( CURLINFO_NAMELOOKUP_TIME_T );
	g_curl_info [ "CURLINFO_CONNECT_TIME" ] = easy_getinfo_double ( CURLINFO_CONNECT_TIME );
	g_curl_info [ "CURLINFO_CONNECT_TIME_T" ] = easy_getinfo_sizet ( CURLINFO_CONNECT_TIME_T );
	g_curl_info [ "CURLINFO_APPCONNECT_TIME" ] = easy_getinfo_double ( CURLINFO_APPCONNECT_TIME );
	g_curl_info [ "CURLINFO_APPCONNECT_TIME_T" ] = easy_getinfo_sizet ( CURLINFO_APPCONNECT_TIME_T );
	g_curl_info [ "CURLINFO_PRETRANSFER_TIME" ] = easy_getinfo_double ( CURLINFO_PRETRANSFER_TIME );
	g_curl_info [ "CURLINFO_PRETRANSFER_TIME_T" ] = easy_getinfo_sizet ( CURLINFO_PRETRANSFER_TIME_T );
	g_curl_info [ "CURLINFO_STARTTRANSFER_TIME" ] = easy_getinfo_double ( CURLINFO_STARTTRANSFER_TIME );
	g_curl_info [ "CURLINFO_STARTTRANSFER_TIME_T" ] = easy_getinfo_sizet ( CURLINFO_STARTTRANSFER_TIME_T );
	g_curl_info [ "CURLINFO_REDIRECT_TIME" ] = easy_getinfo_double ( CURLINFO_REDIRECT_TIME );
	g_curl_info [ "CURLINFO_REDIRECT_TIME_T" ] = easy_getinfo_sizet ( CURLINFO_REDIRECT_TIME_T );
	g_curl_info [ "CURLINFO_REDIRECT_COUNT" ] = easy_getinfo_long ( CURLINFO_REDIRECT_COUNT );
	g_curl_info [ "CURLINFO_REDIRECT_URL" ] = easy_getinfo ( CURLINFO_REDIRECT_URL );
//	g_curl_info [ "CURLINFO_SIZE_UPLOAD" ] = easy_getinfo_double ( CURLINFO_SIZE_UPLOAD ); // deprecated
	g_curl_info [ "CURLINFO_SIZE_UPLOAD_T" ] = easy_getinfo_sizet ( CURLINFO_SIZE_UPLOAD_T );
//	g_curl_info [ "CURLINFO_SIZE_DOWNLOAD" ] = easy_getinfo_double ( CURLINFO_SIZE_DOWNLOAD ); // deprecated
	g_curl_info [ "CURLINFO_SIZE_DOWNLOAD_T" ] = easy_getinfo_sizet ( CURLINFO_SIZE_DOWNLOAD_T );
//	g_curl_info [ "CURLINFO_SPEED_DOWNLOAD" ] = easy_getinfo_double ( CURLINFO_SPEED_DOWNLOAD ); // deprecated
	g_curl_info [ "CURLINFO_SPEED_DOWNLOAD_T" ] = easy_getinfo_sizet ( CURLINFO_SPEED_DOWNLOAD_T );
//	g_curl_info [ "CURLINFO_SPEED_UPLOAD" ] = easy_getinfo_double ( CURLINFO_SPEED_UPLOAD ); // deprecated
	g_curl_info [ "CURLINFO_SPEED_UPLOAD_T" ] = easy_getinfo_sizet ( CURLINFO_SPEED_UPLOAD_T );
	g_curl_info [ "CURLINFO_HEADER_SIZE" ] = easy_getinfo_long ( CURLINFO_HEADER_SIZE );
	g_curl_info [ "CURLINFO_REQUEST_SIZE" ] = easy_getinfo_long ( CURLINFO_REQUEST_SIZE );
	g_curl_info [ "CURLINFO_SSL_VERIFYRESULT" ] = easy_getinfo_long ( CURLINFO_SSL_VERIFYRESULT );
	g_curl_info [ "CURLINFO_PROXY_ERROR" ] = easy_getinfo_long ( CURLINFO_PROXY_ERROR );
	g_curl_info [ "CURLINFO_PROXY_SSL_VERIFYRESULT" ] = easy_getinfo_long ( CURLINFO_PROXY_SSL_VERIFYRESULT );
	g_curl_info [ "CURLINFO_SSL_ENGINES" ] = easy_getinfo_slist ( CURLINFO_SSL_ENGINES );
//	g_curl_info [ "CURLINFO_CONTENT_LENGTH_DOWNLOAD" ] = easy_getinfo_double ( CURLINFO_CONTENT_LENGTH_DOWNLOAD ); // deprecated
	g_curl_info [ "CURLINFO_CONTENT_LENGTH_DOWNLOAD_T" ] = easy_getinfo_sizet ( CURLINFO_CONTENT_LENGTH_DOWNLOAD_T );
//	g_curl_info [ "CURLINFO_CONTENT_LENGTH_UPLOAD" ] = easy_getinfo_double ( CURLINFO_CONTENT_LENGTH_UPLOAD ); // deprecated
	g_curl_info [ "CURLINFO_CONTENT_LENGTH_UPLOAD_T" ] = easy_getinfo_sizet ( CURLINFO_CONTENT_LENGTH_UPLOAD_T );
	g_curl_info [ "CURLINFO_CONTENT_TYPE" ] = easy_getinfo ( CURLINFO_CONTENT_TYPE );
	g_curl_info [ "CURLINFO_RETRY_AFTER" ] = easy_getinfo_sizet ( CURLINFO_RETRY_AFTER );
	g_curl_info [ "CURLINFO_PRIVATE" ] = easy_getinfo ( CURLINFO_PRIVATE );
	g_curl_info [ "CURLINFO_HTTPAUTH_AVAIL" ] = easy_getinfo_long ( CURLINFO_HTTPAUTH_AVAIL );
	g_curl_info [ "CURLINFO_PROXYAUTH_AVAIL" ] = easy_getinfo_long ( CURLINFO_PROXYAUTH_AVAIL );
	g_curl_info [ "CURLINFO_OS_ERRNO" ] = easy_getinfo_long ( CURLINFO_OS_ERRNO );
	g_curl_info [ "CURLINFO_NUM_CONNECTS" ] = easy_getinfo_long ( CURLINFO_NUM_CONNECTS );
	g_curl_info [ "CURLINFO_PRIMARY_IP" ] = easy_getinfo ( CURLINFO_PRIMARY_IP );
	g_curl_info [ "CURLINFO_PRIMARY_PORT" ] = easy_getinfo_long ( CURLINFO_PRIMARY_PORT );
	g_curl_info [ "CURLINFO_LOCAL_IP" ] = easy_getinfo ( CURLINFO_LOCAL_IP );
	g_curl_info [ "CURLINFO_LOCAL_PORT" ] = easy_getinfo_long ( CURLINFO_LOCAL_PORT );
	g_curl_info [ "CURLINFO_COOKIELIST" ] = easy_getinfo_slist ( CURLINFO_COOKIELIST );
//	g_curl_info [ "CURLINFO_LASTSOCKET" ] = easy_getinfo_long ( CURLINFO_LASTSOCKET ); // deprecated
	g_curl_info [ "CURLINFO_ACTIVESOCKET" ] = easy_getinfo_long ( CURLINFO_ACTIVESOCKET ); // not useful in this context ???
	g_curl_info [ "CURLINFO_FTP_ENTRY_PATH" ] = easy_getinfo ( CURLINFO_FTP_ENTRY_PATH );
	g_curl_info [ "CURLINFO_CERTINFO" ] = easy_getinfo_cert ( CURLINFO_CERTINFO );
	g_curl_info [ "CURLINFO_TLS_SSL_PTR" ] = std::to_string ( kCommandIsUnavailableError ); // not implemented
//	g_curl_info [ "CURLINFO_TLS_SESSION" ] = easy_getinfo ( CURLINFO_TLS_SESSION ); // deprecated
	g_curl_info [ "CURLINFO_CONDITION_UNMET" ] = easy_getinfo_long ( CURLINFO_CONDITION_UNMET );
	g_curl_info [ "CURLINFO_RTSP_SESSION_ID" ] = easy_getinfo ( CURLINFO_RTSP_SESSION_ID );
	g_curl_info [ "CURLINFO_RTSP_CLIENT_CSEQ" ] = easy_getinfo_long ( CURLINFO_RTSP_CLIENT_CSEQ );
	g_curl_info [ "CURLINFO_RTSP_SERVER_CSEQ" ] = easy_getinfo_long ( CURLINFO_RTSP_SERVER_CSEQ );
	g_curl_info [ "CURLINFO_RTSP_CSEQ_RECV" ] = easy_getinfo_long ( CURLINFO_RTSP_CSEQ_RECV );
	g_curl_info [ "CURLINFO_PROTOCOL" ] = easy_getinfo_long ( CURLINFO_PROTOCOL );
	g_curl_info [ "CURLINFO_SCHEME" ] = easy_getinfo ( CURLINFO_SCHEME );

}	// get_curl_info

