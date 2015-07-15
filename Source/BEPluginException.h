/*
 BEPluginException.h
 BaseElements Plug-In
 
 Copyright 2014-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef BaseElements_BEPluginException_h
#define BaseElements_BEPluginException_h


#include "BEPluginGlobalDefines.h"


class BEPlugin_Exception : public std::runtime_error {
	
public:

	BEPlugin_Exception ( const fmx::errcode _code ) : runtime_error ( "BEPlugin_Exception" ) { error_code = _code; }
	BEPlugin_Exception ( const fmx::errcode _code, const std::string message ) : runtime_error ( message ) { error_code = _code; }

	virtual const fmx::errcode code() const throw() {
		return error_code;
	}
	
private:
	
	fmx::errcode error_code;
	
};


#endif
