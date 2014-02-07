/*
 BECurlOptions.h
 BaseElements Plug-In
 
 Copyright 2013-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BECurlOptions__
#define __BaseElements__BECurlOptions__


#include "BEPluginGlobalDefines.h"


#include "Curl/Curl.h"


class BECurlOption_Exception : public std::runtime_error {
	
public:
	BECurlOption_Exception ( const int _code ) : runtime_error ( "BECurlOption_Exception" ) { error_code = _code; }
	
	virtual const int code() const throw()
	{
		return error_code;
	}
	
private:
	int error_code;
	
};




class BECurlOption {
	
	
public:
	
	enum BECurlOptionType {
		type_unknown = -1,
		type_not_handled = 0,
		type_string = 1,
		type_long = 2,
		type_curl_off_t = 3,
		type_void_at = 4
	};
	
	BECurlOption ( const std::string option_name, const fmx::Data& option_value );
	
	void set ( const fmx::Data& option_value );
	
	CURLoption option ( );
	BECurlOptionType type ( );
	
	const std::string as_string ( );
	const long as_long ( );
	const curl_off_t as_curl_off_t ( );

	
protected:
		
	std::string name;
	BECurlOptionType option_type;
	
	long long_value;
	std::string string_value;
	curl_off_t curl_off_t_value;
	void * void_at_value;
	
};


#endif /* defined(__BaseElements__BECurlOptions__) */
