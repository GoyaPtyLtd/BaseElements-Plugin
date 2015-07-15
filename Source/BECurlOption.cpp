/*
 BECurlOptions.cpp
 BaseElements Plug-In
 
 Copyright 2013-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */



#include "BECurlOption.h"
#include "BEPluginUtilities.h"

#include "boost/algorithm/string.hpp"

#include <map>


BECurlOption::BECurlOption ( const std::string option_name, const fmx::Data& option_value )
{
	name = option_name;

	option_type = type ( );
	set ( option_value );

}



void BECurlOption::set ( const fmx::Data& option_value )
{

	switch ( option_type ) {
			
		case type_string:
			string_value = DataAsUTF8String ( option_value );
			break;
			
		case type_long:
			long_value = DataAsLong ( option_value );
			break;
			
		case type_curl_off_t:
			curl_off_t_value = DataAsDouble ( option_value );
			break;
			
		case type_named_constant:
			long_value = named_constants ( DataAsUTF8String ( option_value ) );
			break;
			
		default: // type_void_at, type_unknown & type_not_handled
			throw BECurlOption_Exception ( kNotImplemented );
			break;
	}

}


const std::string BECurlOption::as_string ( )
{
	return string_value;
}


const long BECurlOption::as_long ( )
{
	return long_value;
}


const curl_off_t BECurlOption::as_curl_off_t ( )
{
	return curl_off_t_value;
}


const long BECurlOption::named_constants ( const std::string constant_name )
{
	std::map<std::string, long> named_constants;

	named_constants [ "CURLAUTH_BASIC" ] = CURLAUTH_BASIC;
	named_constants [ "CURLAUTH_DIGEST" ] = CURLAUTH_DIGEST;
	named_constants [ "CURLAUTH_DIGEST_IE" ] = CURLAUTH_DIGEST_IE;
	named_constants [ "CURLAUTH_NEGOTIATE" ] = CURLAUTH_NEGOTIATE;
	named_constants [ "CURLAUTH_NTLM" ] = CURLAUTH_NTLM;
	named_constants [ "CURLAUTH_NTLM_WB" ] = CURLAUTH_NTLM_WB;
	named_constants [ "CURLAUTH_ANY" ] = CURLAUTH_ANY;
	named_constants [ "CURLAUTH_ANYSAFE" ] = CURLAUTH_ANYSAFE;
	named_constants [ "CURLAUTH_ONLY" ] = CURLAUTH_ONLY;

	long out = 0;
	
	// look up the constant or, for backward compatibility, try to convert it to an integer
	try {
		out = named_constants.at ( constant_name );
	} catch ( std::out_of_range e ) {
		out = atol ( constant_name.c_str() );
	}
	
	return out;
}



BECurlOption::BECurlOptionType BECurlOption::type ( )
{
	std::map<std::string, BECurlOptionType> types;
	
//	don't allow the url to overridden'
//	types [ "CURLOPT_URL" ] = type_string;
	types [ "CURLOPT_URL" ] = type_not_handled;
	
	types [ "CURLOPT_PROXY" ] = type_string;
	types [ "CURLOPT_NOPROXY" ] = type_string;
	types [ "CURLOPT_SOCKS5_GSSAPI_SERVICE" ] = type_string;
	types [ "CURLOPT_INTERFACE" ] = type_string;
	types [ "CURLOPT_NETRC_FILE" ] = type_string;
	types [ "CURLOPT_USERPWD" ] = type_string;
	types [ "CURLOPT_PROXYUSERPWD" ] = type_string;
	types [ "CURLOPT_USERNAME" ] = type_string;
	types [ "CURLOPT_PASSWORD" ] = type_string;
	types [ "CURLOPT_PROXYUSERNAME" ] = type_string;
	types [ "CURLOPT_PROXYPASSWORD" ] = type_string;
	types [ "CURLOPT_TLSAUTH_USERNAME" ] = type_string;
	types [ "CURLOPT_TLSAUTH_PASSWORD" ] = type_string;
	types [ "CURLOPT_ACCEPT_ENCODING" ] = type_string;
	types [ "CURLOPT_COPYPOSTFIELDS" ] = type_string;
	types [ "CURLOPT_REFERER" ] = type_string;
	types [ "CURLOPT_USERAGENT" ] = type_string;
	types [ "CURLOPT_COOKIE" ] = type_string;
	types [ "CURLOPT_COOKIEFILE" ] = type_string;
	types [ "CURLOPT_COOKIEJAR" ] = type_string;
	types [ "CURLOPT_COOKIELIST" ] = type_string;
	types [ "CURLOPT_HTTPGET" ] = type_string;
	types [ "CURLOPT_MAIL_FROM" ] = type_string;
	types [ "CURLOPT_MAIL_AUTH" ] = type_string;
	types [ "CURLOPT_FTPPORT" ] = type_string;
	types [ "CURLOPT_FTP_ALTERNATIVE_TO_USER" ] = type_string;
	types [ "CURLOPT_FTP_ACCOUNT" ] = type_string;
	types [ "CURLOPT_RTSP_SESSION_ID" ] = type_string;
	types [ "CURLOPT_RTSP_STREAM_URI" ] = type_string;
	types [ "CURLOPT_RTSP_TRANSPORT" ] = type_string;
	types [ "CURLOPT_RANGE" ] = type_string;
	types [ "CURLOPT_CUSTOMREQUEST" ] = type_string;
	types [ "CURLOPT_DNS_SERVERS" ] = type_string;
	types [ "CURLOPT_SSLCERT" ] = type_string;
	types [ "CURLOPT_SSLCERTTYPE" ] = type_string;
	types [ "CURLOPT_SSLKEY" ] = type_string;
	types [ "CURLOPT_SSLKEYTYPE" ] = type_string;
	types [ "CURLOPT_KEYPASSWD" ] = type_string;
	types [ "CURLOPT_SSLENGINE" ] = type_string;
	types [ "CURLOPT_CAINFO" ] = type_string;
	types [ "CURLOPT_ISSUERCERT" ] = type_string;
	types [ "CURLOPT_CAPATH" ] = type_string;
	types [ "CURLOPT_CRLFILE" ] = type_string;
	types [ "CURLOPT_RANDOM_FILE" ] = type_string;
	types [ "CURLOPT_EGDSOCKET" ] = type_string;
	types [ "CURLOPT_SSL_CIPHER_LIST" ] = type_string;
	types [ "CURLOPT_KRBLEVEL" ] = type_string;
	types [ "CURLOPT_SSH_HOST_PUBLIC_KEY_MD5" ] = type_string;
	types [ "CURLOPT_SSH_PUBLIC_KEYFILE" ] = type_string;
	types [ "CURLOPT_SSH_PRIVATE_KEYFILE" ] = type_string;
	types [ "CURLOPT_SSH_KNOWNHOSTS" ] = type_string;

	types [ "CURLOPT_VERBOSE" ] = type_long;
	types [ "CURLOPT_HEADER" ] = type_long;
	types [ "CURLOPT_NOSIGNAL" ] = type_long;
	types [ "CURLOPT_WILDCARDMATCH" ] = type_long;
	types [ "CURLOPT_FAILONERROR" ] = type_long;
	types [ "CURLOPT_PROXYPORT" ] = type_long;
	types [ "CURLOPT_PROXYTYPE" ] = type_long;
	types [ "CURLOPT_HTTPPROXYTUNNEL" ] = type_long;
	types [ "CURLOPT_SOCKS5_GSSAPI_NEC" ] = type_long;
	types [ "CURLOPT_LOCALPORT" ] = type_long;
	types [ "CURLOPT_LOCALPORTRANGE" ] = type_long;
	types [ "CURLOPT_DNS_CACHE_TIMEOUT" ] = type_long;
	types [ "CURLOPT_DNS_USE_GLOBAL_CACHE" ] = type_long;
	types [ "CURLOPT_BUFFERSIZE" ] = type_long;
	types [ "CURLOPT_PORT" ] = type_long;
	types [ "CURLOPT_TCP_NODELAY" ] = type_long;
	types [ "CURLOPT_ADDRESS_SCOPE" ] = type_long;
	types [ "CURLOPT_TCP_KEEPALIVE" ] = type_long;
	types [ "CURLOPT_TCP_KEEPIDLE" ] = type_long;
	types [ "CURLOPT_TCP_KEEPINTVL" ] = type_long;
	types [ "CURLOPT_NETRC" ] = type_long;
	types [ "CURLOPT_HTTPAUTH" ] = type_named_constant;
	types [ "CURLOPT_TLSAUTH_TYPE" ] = type_long;
	types [ "CURLOPT_PROXYAUTH" ] = type_named_constant;
	types [ "CURLOPT_AUTOREFERER" ] = type_long;
	types [ "CURLOPT_TRANSFER_ENCODING" ] = type_long;
	types [ "CURLOPT_FOLLOWLOCATION" ] = type_long;
	types [ "CURLOPT_UNRESTRICTED_AUTH" ] = type_long;
	types [ "CURLOPT_MAXREDIRS" ] = type_long;
	types [ "CURLOPT_PUT" ] = type_long;
	types [ "CURLOPT_POST" ] = type_long;
	types [ "CURLOPT_POSTFIELDSIZE" ] = type_long;
	types [ "CURLOPT_COOKIESESSION" ] = type_long;
	types [ "CURLOPT_HTTP_VERSION" ] = type_long;
	types [ "CURLOPT_IGNORE_CONTENT_LENGTH" ] = type_long;
	types [ "CURLOPT_HTTP_CONTENT_DECODING" ] = type_long;
	types [ "CURLOPT_HTTP_TRANSFER_DECODING" ] = type_long;
	types [ "CURLOPT_TFTP_BLKSIZE" ] = type_long;
	types [ "CURLOPT_FTPPORT" ] = type_long;
	types [ "CURLOPT_DIRLISTONLY" ] = type_long;
	types [ "CURLOPT_APPEND" ] = type_long;
	types [ "CURLOPT_FTP_USE_EPRT" ] = type_long;
	types [ "CURLOPT_FTP_USE_EPSV" ] = type_long;
	types [ "CURLOPT_FTP_USE_PRET" ] = type_long;
	types [ "CURLOPT_FTP_CREATE_MISSING_DIRS" ] = type_long;
	types [ "CURLOPT_FTP_RESPONSE_TIMEOUT" ] = type_long;
	types [ "CURLOPT_FTP_SKIP_PASV_IP" ] = type_long;
	types [ "CURLOPT_FTPSSLAUTH" ] = type_long;
	types [ "CURLOPT_FTP_SSL_CCC" ] = type_long;
	types [ "CURLOPT_FTP_FILEMETHOD" ] = type_long;
	types [ "CURLOPT_RTSP_REQUEST" ] = type_long;
	types [ "CURLOPT_RTSP_CLIENT_CSEQ" ] = type_long;
	types [ "CURLOPT_RTSP_SERVER_CSEQ" ] = type_long;
	types [ "CURLOPT_TRANSFERTEXT" ] = type_long;
	types [ "CURLOPT_PROXY_TRANSFER_MODE" ] = type_long;
	types [ "CURLOPT_CRLF" ] = type_long;
	types [ "CURLOPT_RESUME_FROM" ] = type_long;
	types [ "CURLOPT_FILETIME" ] = type_long;
	types [ "CURLOPT_NOBODY" ] = type_long;
	types [ "CURLOPT_INFILESIZE" ] = type_long;
	types [ "CURLOPT_UPLOAD" ] = type_long;
	types [ "CURLOPT_MAXFILESIZE" ] = type_long;
	types [ "CURLOPT_TIMECONDITION" ] = type_long;
	types [ "CURLOPT_TIMEVALUE" ] = type_long;
	types [ "CURLOPT_TIMEOUT" ] = type_long;
	types [ "CURLOPT_TIMEOUT_MS" ] = type_long;
	types [ "CURLOPT_LOW_SPEED_TIME" ] = type_long;
	types [ "CURLOPT_MAXCONNECTS" ] = type_long;
	types [ "CURLOPT_FRESH_CONNECT" ] = type_long;
	types [ "CURLOPT_FORBID_REUSE" ] = type_long;
	types [ "CURLOPT_CONNECTTIMEOUT" ] = type_long;
	types [ "CURLOPT_CONNECTTIMEOUT_MS" ] = type_long;
	types [ "CURLOPT_IPRESOLVE" ] = type_long;
	types [ "CURLOPT_CONNECT_ONLY" ] = type_long;
	types [ "CURLOPT_USE_SSL" ] = type_long;
	types [ "CURLOPT_ACCEPTTIMEOUT_MS" ] = type_long;
	types [ "CURLOPT_SSLENGINE_DEFAULT" ] = type_long;
	types [ "CURLOPT_SSLVERSION" ] = type_long;
	types [ "CURLOPT_SSL_VERIFYPEER" ] = type_long;
	types [ "CURLOPT_SSL_VERIFYHOST" ] = type_long;
	types [ "CURLOPT_CERTINFO" ] = type_long;
	types [ "CURLOPT_SSL_SESSIONID_CACHE" ] = type_long;
	types [ "CURLOPT_GSSAPI_DELEGATION" ] = type_long;
	types [ "CURLOPT_SSH_AUTH_TYPES" ] = type_long;
	types [ "CURLOPT_NEW_FILE_PERMS" ] = type_long;
	types [ "CURLOPT_NEW_DIRECTORY_PERMS" ] = type_long;
	
	types [ "CURLOPT_NOPROGRESS" ] = type_long;

	types [ "CURLOPT_POSTFIELDSIZE_LARGE" ] = type_curl_off_t;
	types [ "CURLOPT_RESUME_FROM_LARGE" ] = type_curl_off_t;
	types [ "CURLOPT_INFILESIZE_LARGE" ] = type_curl_off_t;
	types [ "CURLOPT_MAXFILESIZE_LARGE" ] = type_curl_off_t;
	types [ "CURLOPT_MAX_SEND_SPEED_LARGE" ] = type_curl_off_t;
	types [ "CURLOPT_MAX_RECV_SPEED_LARGE" ] = type_curl_off_t;

	types [ "CURLOPT_POSTFIELDS" ] = type_void_at;
	types [ "CURLOPT_SSH_KEYDATA" ] = type_void_at;
	types [ "CURLOPT_PRIVATE" ] = type_void_at;

	types [ "CURLOPT_WRITEFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_WRITEDATA" ] = type_not_handled;
	types [ "CURLOPT_READFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_READDATA" ] = type_not_handled;
	types [ "CURLOPT_IOCTLFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_IOCTLDATA" ] = type_not_handled;
	types [ "CURLOPT_SEEKFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_SEEKDATA" ] = type_not_handled;
	types [ "CURLOPT_SOCKOPTFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_SOCKOPTDATA" ] = type_not_handled;
	types [ "CURLOPT_OPENSOCKETFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_OPENSOCKETDATA" ] = type_not_handled;
	types [ "CURLOPT_CLOSESOCKETFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_CLOSESOCKETDATA" ] = type_not_handled;
	types [ "CURLOPT_PROGRESSFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_PROGRESSDATA" ] = type_not_handled;
	types [ "CURLOPT_HEADERFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_WRITEHEADER" ] = type_not_handled;
	types [ "CURLOPT_DEBUGFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_DEBUGDATA" ] = type_not_handled;
	types [ "CURLOPT_SSL_CTX_FUNCTION" ] = type_not_handled;
	types [ "CURLOPT_SSL_CTX_DATA" ] = type_not_handled;
	types [ "CURLOPT_CONV_TO_NETWORK_FUNCTION" ] = type_not_handled;
	types [ "CURLOPT_CONV_FROM_NETWORK_FUNCTION" ] = type_not_handled;
	types [ "CURLOPT_CONV_FROM_UTF8_FUNCTION" ] = type_not_handled;
	types [ "CURLOPT_INTERLEAVEFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_INTERLEAVEDATA" ] = type_not_handled;
	types [ "CURLOPT_CHUNK_BGN_FUNCTION" ] = type_not_handled;
	types [ "CURLOPT_CHUNK_END_FUNCTION" ] = type_not_handled;
	types [ "CURLOPT_CHUNK_DATA" ] = type_not_handled;
	types [ "CURLOPT_FNMATCH_FUNCTION" ] = type_not_handled;
	types [ "CURLOPT_FNMATCH_DATA" ] = type_not_handled;
	types [ "CURLOPT_ERRORBUFFER" ] = type_not_handled;
	types [ "CURLOPT_STDERR" ] = type_not_handled;
	types [ "CURLOPT_FAILONERROR" ] = type_not_handled;
	types [ "CURLOPT_PROTOCOLS" ] = type_not_handled;
	types [ "CURLOPT_REDIR_PROTOCOLS" ] = type_not_handled;
	types [ "CURLOPT_POSTREDIR" ] = type_not_handled;
	types [ "CURLOPT_HTTPPOST" ] = type_not_handled;
	types [ "CURLOPT_HTTPHEADER" ] = type_not_handled;
	types [ "CURLOPT_HTTP200ALIASES" ] = type_not_handled;
	types [ "CURLOPT_MAIL_RCPT" ] = type_not_handled;
	types [ "CURLOPT_QUOTE" ] = type_not_handled;
	types [ "CURLOPT_POSTQUOTE" ] = type_not_handled;
	types [ "CURLOPT_PREQUOTE" ] = type_not_handled;
	types [ "CURLOPT_RTSP_HEADER" ] = type_not_handled;
	types [ "CURLOPT_CLOSEPOLICY" ] = type_not_handled;
	types [ "CURLOPT_RESOLVE" ] = type_not_handled;
	types [ "CURLOPT_SSL_OPTIONS" ] = type_not_handled;
	types [ "CURLOPT_SSH_KEYFUNCTION" ] = type_not_handled;
	types [ "CURLOPT_SHARE" ] = type_not_handled;
	types [ "CURLOPT_TELNETOPTIONS" ] = type_not_handled;
	
//	BECurlOptionType type = type_unknown;
	return types.at ( boost::to_upper_copy ( name ) ); // note: at throws if not found
	
}



CURLoption BECurlOption::option ( )
{
	std::map<std::string, CURLoption> options;
	
	options [ "CURLOPT_POSTFIELDSIZE_LARGE" ] = CURLOPT_POSTFIELDSIZE_LARGE;
	options [ "CURLOPT_RESUME_FROM_LARGE" ] = CURLOPT_RESUME_FROM_LARGE;
	options [ "CURLOPT_INFILESIZE_LARGE" ] = CURLOPT_INFILESIZE_LARGE;
	options [ "CURLOPT_MAXFILESIZE_LARGE" ] = CURLOPT_MAXFILESIZE_LARGE;
	options [ "CURLOPT_MAX_SEND_SPEED_LARGE" ] = CURLOPT_MAX_SEND_SPEED_LARGE;
	options [ "CURLOPT_MAX_RECV_SPEED_LARGE" ] = CURLOPT_MAX_RECV_SPEED_LARGE;
	options [ "CURLOPT_URL" ] = CURLOPT_URL;
	options [ "CURLOPT_PROXY" ] = CURLOPT_PROXY;
	options [ "CURLOPT_NOPROXY" ] = CURLOPT_NOPROXY;
	options [ "CURLOPT_SOCKS5_GSSAPI_SERVICE" ] = CURLOPT_SOCKS5_GSSAPI_SERVICE;
	options [ "CURLOPT_INTERFACE" ] = CURLOPT_INTERFACE;
	options [ "CURLOPT_NETRC_FILE" ] = CURLOPT_NETRC_FILE;
	options [ "CURLOPT_USERPWD" ] = CURLOPT_USERPWD;
	options [ "CURLOPT_PROXYUSERPWD" ] = CURLOPT_PROXYUSERPWD;
	options [ "CURLOPT_USERNAME" ] = CURLOPT_USERNAME;
	options [ "CURLOPT_PASSWORD" ] = CURLOPT_PASSWORD;
	options [ "CURLOPT_PROXYUSERNAME" ] = CURLOPT_PROXYUSERNAME;
	options [ "CURLOPT_PROXYPASSWORD" ] = CURLOPT_PROXYPASSWORD;
	options [ "CURLOPT_TLSAUTH_USERNAME" ] = CURLOPT_TLSAUTH_USERNAME;
	options [ "CURLOPT_TLSAUTH_PASSWORD" ] = CURLOPT_TLSAUTH_PASSWORD;
	options [ "CURLOPT_ACCEPT_ENCODING" ] = CURLOPT_ACCEPT_ENCODING;
	options [ "CURLOPT_COPYPOSTFIELDS" ] = CURLOPT_COPYPOSTFIELDS;
	options [ "CURLOPT_REFERER" ] = CURLOPT_REFERER;
	options [ "CURLOPT_USERAGENT" ] = CURLOPT_USERAGENT;
	options [ "CURLOPT_COOKIE" ] = CURLOPT_COOKIE;
	options [ "CURLOPT_COOKIEFILE" ] = CURLOPT_COOKIEFILE;
	options [ "CURLOPT_COOKIEJAR" ] = CURLOPT_COOKIEJAR;
	options [ "CURLOPT_COOKIELIST" ] = CURLOPT_COOKIELIST;
	options [ "CURLOPT_HTTPGET" ] = CURLOPT_HTTPGET;
	options [ "CURLOPT_MAIL_FROM" ] = CURLOPT_MAIL_FROM;
	options [ "CURLOPT_MAIL_AUTH" ] = CURLOPT_MAIL_AUTH;
	options [ "CURLOPT_FTPPORT" ] = CURLOPT_FTPPORT;
	options [ "CURLOPT_FTP_ALTERNATIVE_TO_USER" ] = CURLOPT_FTP_ALTERNATIVE_TO_USER;
	options [ "CURLOPT_FTP_ACCOUNT" ] = CURLOPT_FTP_ACCOUNT;
	options [ "CURLOPT_RTSP_SESSION_ID" ] = CURLOPT_RTSP_SESSION_ID;
	options [ "CURLOPT_RTSP_STREAM_URI" ] = CURLOPT_RTSP_STREAM_URI;
	options [ "CURLOPT_RTSP_TRANSPORT" ] = CURLOPT_RTSP_TRANSPORT;
	options [ "CURLOPT_RANGE" ] = CURLOPT_RANGE;
	options [ "CURLOPT_CUSTOMREQUEST" ] = CURLOPT_CUSTOMREQUEST;
	options [ "CURLOPT_DNS_SERVERS" ] = CURLOPT_DNS_SERVERS;
	options [ "CURLOPT_SSLCERT" ] = CURLOPT_SSLCERT;
	options [ "CURLOPT_SSLCERTTYPE" ] = CURLOPT_SSLCERTTYPE;
	options [ "CURLOPT_SSLKEY" ] = CURLOPT_SSLKEY;
	options [ "CURLOPT_SSLKEYTYPE" ] = CURLOPT_SSLKEYTYPE;
	options [ "CURLOPT_KEYPASSWD" ] = CURLOPT_KEYPASSWD;
	options [ "CURLOPT_SSLENGINE" ] = CURLOPT_SSLENGINE;
	options [ "CURLOPT_CAINFO" ] = CURLOPT_CAINFO;
	options [ "CURLOPT_ISSUERCERT" ] = CURLOPT_ISSUERCERT;
	options [ "CURLOPT_CAPATH" ] = CURLOPT_CAPATH;
	options [ "CURLOPT_CRLFILE" ] = CURLOPT_CRLFILE;
	options [ "CURLOPT_RANDOM_FILE" ] = CURLOPT_RANDOM_FILE;
	options [ "CURLOPT_EGDSOCKET" ] = CURLOPT_EGDSOCKET;
	options [ "CURLOPT_SSL_CIPHER_LIST" ] = CURLOPT_SSL_CIPHER_LIST;
	options [ "CURLOPT_KRBLEVEL" ] = CURLOPT_KRBLEVEL;
	options [ "CURLOPT_SSH_HOST_PUBLIC_KEY_MD5" ] = CURLOPT_SSH_HOST_PUBLIC_KEY_MD5;
	options [ "CURLOPT_SSH_PUBLIC_KEYFILE" ] = CURLOPT_SSH_PUBLIC_KEYFILE;
	options [ "CURLOPT_SSH_PRIVATE_KEYFILE" ] = CURLOPT_SSH_PRIVATE_KEYFILE;
	options [ "CURLOPT_SSH_KNOWNHOSTS" ] = CURLOPT_SSH_KNOWNHOSTS;
	options [ "CURLOPT_POSTFIELDS" ] = CURLOPT_POSTFIELDS;
	options [ "CURLOPT_SSH_KEYDATA" ] = CURLOPT_SSH_KEYDATA;
	options [ "CURLOPT_PRIVATE" ] = CURLOPT_PRIVATE;
	options [ "CURLOPT_VERBOSE" ] = CURLOPT_VERBOSE;
	options [ "CURLOPT_HEADER" ] = CURLOPT_HEADER;
	options [ "CURLOPT_NOSIGNAL" ] = CURLOPT_NOSIGNAL;
	options [ "CURLOPT_WILDCARDMATCH" ] = CURLOPT_WILDCARDMATCH;
	options [ "CURLOPT_FAILONERROR" ] = CURLOPT_FAILONERROR;
	options [ "CURLOPT_PROXYPORT" ] = CURLOPT_PROXYPORT;
	options [ "CURLOPT_PROXYTYPE" ] = CURLOPT_PROXYTYPE;
	options [ "CURLOPT_HTTPPROXYTUNNEL" ] = CURLOPT_HTTPPROXYTUNNEL;
	options [ "CURLOPT_SOCKS5_GSSAPI_NEC" ] = CURLOPT_SOCKS5_GSSAPI_NEC;
	options [ "CURLOPT_LOCALPORT" ] = CURLOPT_LOCALPORT;
	options [ "CURLOPT_LOCALPORTRANGE" ] = CURLOPT_LOCALPORTRANGE;
	options [ "CURLOPT_DNS_CACHE_TIMEOUT" ] = CURLOPT_DNS_CACHE_TIMEOUT;
	options [ "CURLOPT_DNS_USE_GLOBAL_CACHE" ] = CURLOPT_DNS_USE_GLOBAL_CACHE;
	options [ "CURLOPT_BUFFERSIZE" ] = CURLOPT_BUFFERSIZE;
	options [ "CURLOPT_PORT" ] = CURLOPT_PORT;
	options [ "CURLOPT_TCP_NODELAY" ] = CURLOPT_TCP_NODELAY;
	options [ "CURLOPT_ADDRESS_SCOPE" ] = CURLOPT_ADDRESS_SCOPE;
	options [ "CURLOPT_TCP_KEEPALIVE" ] = CURLOPT_TCP_KEEPALIVE;
	options [ "CURLOPT_TCP_KEEPIDLE" ] = CURLOPT_TCP_KEEPIDLE;
	options [ "CURLOPT_TCP_KEEPINTVL" ] = CURLOPT_TCP_KEEPINTVL;
	options [ "CURLOPT_NETRC" ] = CURLOPT_NETRC;
	options [ "CURLOPT_HTTPAUTH" ] = CURLOPT_HTTPAUTH;
	options [ "CURLOPT_TLSAUTH_TYPE" ] = CURLOPT_TLSAUTH_TYPE;
	options [ "CURLOPT_PROXYAUTH" ] = CURLOPT_PROXYAUTH;
	options [ "CURLOPT_AUTOREFERER" ] = CURLOPT_AUTOREFERER;
	options [ "CURLOPT_TRANSFER_ENCODING" ] = CURLOPT_TRANSFER_ENCODING;
	options [ "CURLOPT_FOLLOWLOCATION" ] = CURLOPT_FOLLOWLOCATION;
	options [ "CURLOPT_UNRESTRICTED_AUTH" ] = CURLOPT_UNRESTRICTED_AUTH;
	options [ "CURLOPT_MAXREDIRS" ] = CURLOPT_MAXREDIRS;
	options [ "CURLOPT_PUT" ] = CURLOPT_PUT;
	options [ "CURLOPT_POST" ] = CURLOPT_POST;
	options [ "CURLOPT_POSTFIELDSIZE" ] = CURLOPT_POSTFIELDSIZE;
	options [ "CURLOPT_COOKIESESSION" ] = CURLOPT_COOKIESESSION;
	options [ "CURLOPT_HTTP_VERSION" ] = CURLOPT_HTTP_VERSION;
	options [ "CURLOPT_IGNORE_CONTENT_LENGTH" ] = CURLOPT_IGNORE_CONTENT_LENGTH;
	options [ "CURLOPT_HTTP_CONTENT_DECODING" ] = CURLOPT_HTTP_CONTENT_DECODING;
	options [ "CURLOPT_HTTP_TRANSFER_DECODING" ] = CURLOPT_HTTP_TRANSFER_DECODING;
	options [ "CURLOPT_TFTP_BLKSIZE" ] = CURLOPT_TFTP_BLKSIZE;
	options [ "CURLOPT_FTPPORT" ] = CURLOPT_FTPPORT;
	options [ "CURLOPT_DIRLISTONLY" ] = CURLOPT_DIRLISTONLY;
	options [ "CURLOPT_APPEND" ] = CURLOPT_APPEND;
	options [ "CURLOPT_FTP_USE_EPRT" ] = CURLOPT_FTP_USE_EPRT;
	options [ "CURLOPT_FTP_USE_EPSV" ] = CURLOPT_FTP_USE_EPSV;
	options [ "CURLOPT_FTP_USE_PRET" ] = CURLOPT_FTP_USE_PRET;
	options [ "CURLOPT_FTP_CREATE_MISSING_DIRS" ] = CURLOPT_FTP_CREATE_MISSING_DIRS;
	options [ "CURLOPT_FTP_RESPONSE_TIMEOUT" ] = CURLOPT_FTP_RESPONSE_TIMEOUT;
	options [ "CURLOPT_FTP_SKIP_PASV_IP" ] = CURLOPT_FTP_SKIP_PASV_IP;
	options [ "CURLOPT_FTPSSLAUTH" ] = CURLOPT_FTPSSLAUTH;
	options [ "CURLOPT_FTP_SSL_CCC" ] = CURLOPT_FTP_SSL_CCC;
	options [ "CURLOPT_FTP_FILEMETHOD" ] = CURLOPT_FTP_FILEMETHOD;
	options [ "CURLOPT_RTSP_REQUEST" ] = CURLOPT_RTSP_REQUEST;
	options [ "CURLOPT_RTSP_CLIENT_CSEQ" ] = CURLOPT_RTSP_CLIENT_CSEQ;
	options [ "CURLOPT_RTSP_SERVER_CSEQ" ] = CURLOPT_RTSP_SERVER_CSEQ;
	options [ "CURLOPT_TRANSFERTEXT" ] = CURLOPT_TRANSFERTEXT;
	options [ "CURLOPT_PROXY_TRANSFER_MODE" ] = CURLOPT_PROXY_TRANSFER_MODE;
	options [ "CURLOPT_CRLF" ] = CURLOPT_CRLF;
	options [ "CURLOPT_RESUME_FROM" ] = CURLOPT_RESUME_FROM;
	options [ "CURLOPT_FILETIME" ] = CURLOPT_FILETIME;
	options [ "CURLOPT_NOBODY" ] = CURLOPT_NOBODY;
	options [ "CURLOPT_INFILESIZE" ] = CURLOPT_INFILESIZE;
	options [ "CURLOPT_UPLOAD" ] = CURLOPT_UPLOAD;
	options [ "CURLOPT_MAXFILESIZE" ] = CURLOPT_MAXFILESIZE;
	options [ "CURLOPT_TIMECONDITION" ] = CURLOPT_TIMECONDITION;
	options [ "CURLOPT_TIMEVALUE" ] = CURLOPT_TIMEVALUE;
	options [ "CURLOPT_TIMEOUT" ] = CURLOPT_TIMEOUT;
	options [ "CURLOPT_TIMEOUT_MS" ] = CURLOPT_TIMEOUT_MS;
	options [ "CURLOPT_LOW_SPEED_TIME" ] = CURLOPT_LOW_SPEED_TIME;
	options [ "CURLOPT_MAXCONNECTS" ] = CURLOPT_MAXCONNECTS;
	options [ "CURLOPT_FRESH_CONNECT" ] = CURLOPT_FRESH_CONNECT;
	options [ "CURLOPT_FORBID_REUSE" ] = CURLOPT_FORBID_REUSE;
	options [ "CURLOPT_CONNECTTIMEOUT" ] = CURLOPT_CONNECTTIMEOUT;
	options [ "CURLOPT_CONNECTTIMEOUT_MS" ] = CURLOPT_CONNECTTIMEOUT_MS;
	options [ "CURLOPT_IPRESOLVE" ] = CURLOPT_IPRESOLVE;
	options [ "CURLOPT_CONNECT_ONLY" ] = CURLOPT_CONNECT_ONLY;
	options [ "CURLOPT_USE_SSL" ] = CURLOPT_USE_SSL;
	options [ "CURLOPT_ACCEPTTIMEOUT_MS" ] = CURLOPT_ACCEPTTIMEOUT_MS;
	options [ "CURLOPT_SSLENGINE_DEFAULT" ] = CURLOPT_SSLENGINE_DEFAULT;
	options [ "CURLOPT_SSLVERSION" ] = CURLOPT_SSLVERSION;
	options [ "CURLOPT_SSL_VERIFYPEER" ] = CURLOPT_SSL_VERIFYPEER;
	options [ "CURLOPT_SSL_VERIFYHOST" ] = CURLOPT_SSL_VERIFYHOST;
	options [ "CURLOPT_CERTINFO" ] = CURLOPT_CERTINFO;
	options [ "CURLOPT_SSL_SESSIONID_CACHE" ] = CURLOPT_SSL_SESSIONID_CACHE;
	options [ "CURLOPT_GSSAPI_DELEGATION" ] = CURLOPT_GSSAPI_DELEGATION;
	options [ "CURLOPT_SSH_AUTH_TYPES" ] = CURLOPT_SSH_AUTH_TYPES;
	options [ "CURLOPT_NEW_FILE_PERMS" ] = CURLOPT_NEW_FILE_PERMS;
	options [ "CURLOPT_NEW_DIRECTORY_PERMS" ] = CURLOPT_NEW_DIRECTORY_PERMS;
	options [ "CURLOPT_WRITEFUNCTION" ] = CURLOPT_WRITEFUNCTION;
	options [ "CURLOPT_WRITEDATA" ] = CURLOPT_WRITEDATA;
	options [ "CURLOPT_READFUNCTION" ] = CURLOPT_READFUNCTION;
	options [ "CURLOPT_READDATA" ] = CURLOPT_READDATA;
	options [ "CURLOPT_IOCTLFUNCTION" ] = CURLOPT_IOCTLFUNCTION;
	options [ "CURLOPT_IOCTLDATA" ] = CURLOPT_IOCTLDATA;
	options [ "CURLOPT_SEEKFUNCTION" ] = CURLOPT_SEEKFUNCTION;
	options [ "CURLOPT_SEEKDATA" ] = CURLOPT_SEEKDATA;
	options [ "CURLOPT_SOCKOPTFUNCTION" ] = CURLOPT_SOCKOPTFUNCTION;
	options [ "CURLOPT_SOCKOPTDATA" ] = CURLOPT_SOCKOPTDATA;
	options [ "CURLOPT_OPENSOCKETFUNCTION" ] = CURLOPT_OPENSOCKETFUNCTION;
	options [ "CURLOPT_OPENSOCKETDATA" ] = CURLOPT_OPENSOCKETDATA;
	options [ "CURLOPT_CLOSESOCKETFUNCTION" ] = CURLOPT_CLOSESOCKETFUNCTION;
	options [ "CURLOPT_CLOSESOCKETDATA" ] = CURLOPT_CLOSESOCKETDATA;
	options [ "CURLOPT_PROGRESSFUNCTION" ] = CURLOPT_PROGRESSFUNCTION;
	options [ "CURLOPT_PROGRESSDATA" ] = CURLOPT_PROGRESSDATA;
	options [ "CURLOPT_HEADERFUNCTION" ] = CURLOPT_HEADERFUNCTION;
	options [ "CURLOPT_WRITEHEADER" ] = CURLOPT_WRITEHEADER;
	options [ "CURLOPT_DEBUGFUNCTION" ] = CURLOPT_DEBUGFUNCTION;
	options [ "CURLOPT_DEBUGDATA" ] = CURLOPT_DEBUGDATA;
	options [ "CURLOPT_SSL_CTX_FUNCTION" ] = CURLOPT_SSL_CTX_FUNCTION;
	options [ "CURLOPT_SSL_CTX_DATA" ] = CURLOPT_SSL_CTX_DATA;
	options [ "CURLOPT_CONV_TO_NETWORK_FUNCTION" ] = CURLOPT_CONV_TO_NETWORK_FUNCTION;
	options [ "CURLOPT_CONV_FROM_NETWORK_FUNCTION" ] = CURLOPT_CONV_FROM_NETWORK_FUNCTION;
	options [ "CURLOPT_CONV_FROM_UTF8_FUNCTION" ] = CURLOPT_CONV_FROM_UTF8_FUNCTION;
	options [ "CURLOPT_INTERLEAVEFUNCTION" ] = CURLOPT_INTERLEAVEFUNCTION;
	options [ "CURLOPT_INTERLEAVEDATA" ] = CURLOPT_INTERLEAVEDATA;
	options [ "CURLOPT_CHUNK_BGN_FUNCTION" ] = CURLOPT_CHUNK_BGN_FUNCTION;
	options [ "CURLOPT_CHUNK_END_FUNCTION" ] = CURLOPT_CHUNK_END_FUNCTION;
	options [ "CURLOPT_CHUNK_DATA" ] = CURLOPT_CHUNK_DATA;
	options [ "CURLOPT_FNMATCH_FUNCTION" ] = CURLOPT_FNMATCH_FUNCTION;
	options [ "CURLOPT_FNMATCH_DATA" ] = CURLOPT_FNMATCH_DATA;
	options [ "CURLOPT_ERRORBUFFER" ] = CURLOPT_ERRORBUFFER;
	options [ "CURLOPT_STDERR" ] = CURLOPT_STDERR;
	options [ "CURLOPT_FAILONERROR" ] = CURLOPT_FAILONERROR;
	options [ "CURLOPT_PROTOCOLS" ] = CURLOPT_PROTOCOLS;
	options [ "CURLOPT_REDIR_PROTOCOLS" ] = CURLOPT_REDIR_PROTOCOLS;
	options [ "CURLOPT_POSTREDIR" ] = CURLOPT_POSTREDIR;
	options [ "CURLOPT_HTTPPOST" ] = CURLOPT_HTTPPOST;
	options [ "CURLOPT_HTTPHEADER" ] = CURLOPT_HTTPHEADER;
	options [ "CURLOPT_HTTP200ALIASES" ] = CURLOPT_HTTP200ALIASES;
	options [ "CURLOPT_MAIL_RCPT" ] = CURLOPT_MAIL_RCPT;
	options [ "CURLOPT_QUOTE" ] = CURLOPT_QUOTE;
	options [ "CURLOPT_POSTQUOTE" ] = CURLOPT_POSTQUOTE;
	options [ "CURLOPT_PREQUOTE" ] = CURLOPT_PREQUOTE;
//	options [ "CURLOPT_RTSP_HEADER" ] = CURLOPT_RTSP_HEADER;
	options [ "CURLOPT_CLOSEPOLICY" ] = CURLOPT_CLOSEPOLICY;
	options [ "CURLOPT_RESOLVE" ] = CURLOPT_RESOLVE;
	options [ "CURLOPT_SSL_OPTIONS" ] = CURLOPT_SSL_OPTIONS;
	options [ "CURLOPT_SSH_KEYFUNCTION" ] = CURLOPT_SSH_KEYFUNCTION;
	options [ "CURLOPT_SHARE" ] = CURLOPT_SHARE;
	options [ "CURLOPT_TELNETOPTIONS" ] = CURLOPT_TELNETOPTIONS;
	options [ "CURLOPT_NOPROGRESS" ] = CURLOPT_NOPROGRESS;
	
	return options.at ( boost::to_upper_copy ( name ) ); // note: at throws if not found
	
}
