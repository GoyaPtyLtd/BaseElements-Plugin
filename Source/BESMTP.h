/*
 BESMTP.h
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
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
	BESMTP ( const std::string host, const std::string port, const std::string username, const std::string password );
	
	fmx::errcode send ( BESMTPEmailMessage * message );
	
	
protected:
	
};


#endif /* defined(__BaseElements__BESMTP__) */
