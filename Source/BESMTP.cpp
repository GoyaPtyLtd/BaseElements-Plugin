/*
 BESMTP.cpp
 BaseElements Plug-In
 
 Copyright 2014-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESMTP.h"
#include "BECppUtilities.h"


extern thread_local BECurlOptionMap g_curl_options;
extern thread_local CustomHeaders g_http_custom_headers;


using namespace std;


const std::string strip_address ( const std::string address );
const std::string strip_address ( const std::string address )
{
    const size_t open = address.find ( "<" );
    const size_t close = address.find ( ">" );
    
    std::string stripped;
    
    if ( open != std::string::npos && close != std::string::npos ) {
        stripped = address.substr ( open, close );
    } else {
        stripped = address;
    }
    
    return stripped;
}


#pragma mark -
#pragma mark Constructors
#pragma mark -


BESMTP::BESMTP ( const std::string _host, const std::string _port, const std::string _username, const std::string _password )
{
	
	username = _username;
	password = _password;
	
	set_username_and_password ( );

	string scheme = "smtp";
	
	// if there's a uername etc. try for SSL
	if ( !username.empty() ) {
		if ( curl_force_tls ( g_curl_options ) == false ) {
			scheme += "s";
		}
		easy_setopt ( CURLOPT_USE_SSL, CURLUSESSL_ALL );
	}
	
	string url = scheme + "://" + join ( _host, _port, ":" );
	easy_setopt ( CURLOPT_URL, url.c_str() );

	// allow the user to override anything we set
	set_options ( g_curl_options );
		
	set_custom_headers ( g_http_custom_headers );

}


#pragma mark -
#pragma mark Methods
#pragma mark -

fmx::errcode BESMTP::send ( BESMTPEmailMessage * message )
{
	CURLcode result = CURLE_OK;

    std::string from = strip_address ( message->from_address() );

    easy_setopt ( CURLOPT_MAIL_FROM, from.c_str() );

	// who we send this to
	
	BEValueListStringUniquePtr send_to = message->to_address();
	send_to->append ( *(message->cc_addresses()) );
	send_to->append ( *(message->bcc_addresses()) );
	
	struct curl_slist * recipients = NULL;
	vector<string> addresses = send_to->get_values();
	for ( vector<string>::iterator it = addresses.begin() ; it != addresses.end() ; ++it ) {
        auto address = strip_address ( *it );
		recipients = curl_slist_append ( recipients, address.c_str() );
	}
		
	if ( recipients ) {
		
		easy_setopt ( CURLOPT_MAIL_RCPT, recipients );
	
		// the payload
		try {
			
			string body_string = message->as_string();

			easy_setopt ( CURLOPT_READFUNCTION, ReadMemoryCallback );
			userdata.memory = (char *)body_string.c_str();
			userdata.size = body_string.size();
			easy_setopt ( CURLOPT_READDATA, &userdata );
			easy_setopt ( CURLOPT_INFILESIZE, userdata.size );
			easy_setopt ( CURLOPT_UPLOAD, 1L );

			// send it
			result = curl_easy_perform ( curl );

		} catch ( BEPlugin_Exception &e ) {
			result = (CURLcode)e.code();
		}

		error = result;

		// clean up
		curl_slist_free_all ( recipients );

	}

	return (fmx::errcode)result;
	
} // send

