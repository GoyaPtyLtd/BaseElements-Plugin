/*
 BESMTP.cpp
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESMTP.h"


using namespace std;


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
		scheme += "s";
		easy_setopt ( CURLOPT_USE_SSL, CURLUSESSL_ALL );
	}
	
	string url = scheme + "://" + join ( _host, _port, ":" );
	easy_setopt ( CURLOPT_URL, url.c_str() );

	
}


#pragma mark -
#pragma mark Methods
#pragma mark -


fmx::errcode BESMTP::send ( BESMTPEmailMessage * message )
{
	CURLcode result = CURLE_OK;
	
	easy_setopt ( CURLOPT_MAIL_FROM, message->from_address().c_str() );

	// who we send this to
	
	struct curl_slist *recipients = NULL;
	BEValueList<string> send_to = message->to_address();
	send_to.append ( message->cc_addresses() );
	send_to.append ( message->bcc_addresses() );
	
	vector<string> addresses = send_to.get_values();
	for ( vector<string>::iterator it = addresses.begin() ; it != addresses.end() ; ++it ) {
		recipients = curl_slist_append ( recipients, (*it).c_str() );
	}
		
	if ( recipients ) {
		
		easy_setopt ( CURLOPT_MAIL_RCPT, recipients );
	
		// the payload
		string body_string = message->as_string();

		easy_setopt ( CURLOPT_READFUNCTION, ReadMemoryCallback );
		userdata.memory = (char *)body_string.c_str();
		userdata.size = body_string.size();
		easy_setopt ( CURLOPT_READDATA, &userdata );
		easy_setopt ( CURLOPT_INFILESIZE, userdata.size );
		easy_setopt ( CURLOPT_UPLOAD, 1L );

		
		// send it
		result = curl_easy_perform ( curl );
		error = result;
	
		// clean up
		curl_slist_free_all ( recipients );
	}
	
	return (fmx::errcode)result;
	
} // send

