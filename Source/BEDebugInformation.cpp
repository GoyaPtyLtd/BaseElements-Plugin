/*
 BEDebugInformation.cpp
 BaseElements Plug-In

 Copyright (c) 2019-2025 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <Poco/JSON/Object.h>
#pragma GCC diagnostic pop

#include "BEDebugInformation.h"

#include "Net/BECurl.h"
#include "Net/BESMTP.h"
#include "BEFileMakerPlugin.h"


#include <chrono>
#include <sstream>

#include <Poco/String.h>


using namespace std;
using namespace fmx;


extern BEFileMakerPlugin * g_be_plugin;

extern thread_local errcode g_last_error;
extern thread_local errcode g_last_ddl_error;
extern thread_local string g_text_encoding;

extern thread_local std::map<short, std::string> g_script_steps;

extern thread_local BESMTPUniquePtr g_smtp_connection;
extern thread_local BESMTPContainerAttachments g_smtp_attachments;
extern thread_local int g_http_response_code;
extern thread_local std::string g_http_response_headers;
extern thread_local stringstream g_curl_trace;
extern thread_local CustomHeaders g_http_custom_headers;
extern thread_local CustomHeaders g_smtp_custom_headers;
extern thread_local struct host_details g_http_proxy;
extern thread_local BECurlOptionMap g_curl_options;


const std::string protect_text ( const std::string& text );
const std::string protect_text ( const std::string& text ) {

	auto protected_text = text;

	const size_t offset = 2;
	auto minimum_length = offset * 2;

	if ( protected_text.length() > minimum_length ) {
		auto replacing = protected_text.length() - minimum_length;
		protected_text.replace ( offset, replacing, replacing, '*' );
	}

	return protected_text;
}


const string debug_information ( const ExprEnv& environment ) {

	Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

	Poco::JSON::Object::Ptr plugin_information = new Poco::JSON::Object();
	plugin_information->set ( "Auto Update Version", AUTO_UPDATE_VERSION );
	plugin_information->set ( "Version", VERSION_STRING );
	
#ifdef BEP_PRO_VERSION
	#define PRO_VERSION_STRING "Yes"
#else
	#define PRO_VERSION_STRING "No"
#endif

	plugin_information->set ( "Pro Version", PRO_VERSION_STRING );
	json->set ( "Plugin Information", plugin_information );

	std::time_t time_now = std::time ( nullptr );
	std::string now = std::ctime ( &time_now );
	Poco::trimRightInPlace ( now );
	json->set ( "Date", now );

	Poco::JSON::Object::Ptr filemaker_information = new Poco::JSON::Object();
	filemaker_information->set ( "Application", g_be_plugin->get_fmx_application() );
	filemaker_information->set ( "Version", g_be_plugin->get_fmx_application_version() );
	filemaker_information->set ( "Session ID", environment.SessionID() );
	filemaker_information->set ( "File ID", environment.FileID() );
	json->set ( "FileMaker Information", filemaker_information );

	json->set ( "Text Encoding", g_text_encoding );
	json->set ( "Last Error", g_last_error );
	json->set ( "Last DDL Error", g_last_ddl_error );


	Poco::JSON::Object::Ptr curl_information = new Poco::JSON::Object();
	curl_information->set ( "HTTP Response Code", g_http_response_code );
	curl_information->set ( "Trace", g_curl_trace.str() );
	curl_information->set ( "HTTP Response Headers", g_http_response_headers );

	if ( g_smtp_connection ) {
		
		auto smtp_connection = g_smtp_connection->host_details();

		Poco::JSON::Object::Ptr smtp_information = new Poco::JSON::Object();
		smtp_information->set ( "Server", smtp_connection.host );
		smtp_information->set ( "Port", smtp_connection.port );
		smtp_information->set ( "Username", smtp_connection.username );
		smtp_information->set ( "Password", protect_text ( smtp_connection.password ) );
		curl_information->set ( "SMTP Server Information", smtp_information );

	}

	Poco::JSON::Object::Ptr http_proxy_information = new Poco::JSON::Object();
	http_proxy_information->set ( "Server", g_http_proxy.host );
	http_proxy_information->set ( "Port", g_http_proxy.port );
	http_proxy_information->set ( "Username", g_http_proxy.username );
	http_proxy_information->set ( "Password", protect_text ( g_http_proxy.password ) );
	curl_information->set ( "HTTP Proxy Information", http_proxy_information );

	Poco::JSON::Object::Ptr smtp_attachments = new Poco::JSON::Object();
	for ( auto const &attachment : g_smtp_attachments.get_file_list() ) {
		smtp_attachments->set ( attachment.first.string(), attachment.second );
	}
	curl_information->set ( "SMTP Attachments", smtp_attachments );

	Poco::JSON::Object::Ptr curl_options = new Poco::JSON::Object();
	for ( auto const &option : g_curl_options ) {
		curl_options->set ( option.first, option.second->as_string() );
	}
	curl_information->set ( "Curl Options", curl_options );

	Poco::JSON::Object::Ptr http_custom_headers = new Poco::JSON::Object();
	for ( auto const &option : g_http_custom_headers ) {
		http_custom_headers->set ( option.first, option.second );
	}
	curl_information->set ( "Custom HTTP Headers", http_custom_headers );

	Poco::JSON::Object::Ptr smtp_custom_headers = new Poco::JSON::Object();
	for ( auto const &option : g_smtp_custom_headers ) {
		smtp_custom_headers->set ( option.first, option.second );
	}
	curl_information->set ( "Custom SMTP Headers", smtp_custom_headers );

	json->set ( "Curl Information", curl_information );


	Poco::JSON::Object::Ptr script_steps = new Poco::JSON::Object();
	for ( auto const &step : g_script_steps ) {
		script_steps->set ( step.second, step.first );
	}
	json->set ( "Script Steps", script_steps );


	const int indent = 1;
	std::ostringstream output;
	json->stringify ( output, indent );

	return output.str();

}

