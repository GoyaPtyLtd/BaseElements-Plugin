/*
 BESMTP.h
 BaseElements Plug-In
 
 Copyright 2014-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BESMTP__
#define __BaseElements__BESMTP__


#include "BEPluginGlobalDefines.h"
#include "BECurl.h"
#include "BESMTPEmailMessage.h"


#include <string>



class BESMTP : public BECurl {
	
public:
	
	explicit BESMTP ( ) : BECurl ( ) { }
	BESMTP ( const std::string host, const std::string port, const std::string username, const std::string password, const bool keep_open = false );
	
	const fmx::errcode send ( BESMTPEmailMessage * message );
	const bool keep_open() { return keep_connection_open; }
	const struct host_details host_details();
	
	
protected:
	
	std::string host;
	std::string port;
	bool keep_connection_open = false;
	
};


typedef std::unique_ptr<BESMTP> BESMTPUniquePtr;


#endif /* defined(__BaseElements__BESMTP__) */
