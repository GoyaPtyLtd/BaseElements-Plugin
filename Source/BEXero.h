/*
 BE_Xero.h
 BaseElements Plug-In

 Copyright (c) 2014~2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#ifndef __BaseElements__BE_Xero__
#define __BaseElements__BE_Xero__

#include "BEOAuth.h"

#include <openssl/rsa.h>


const std::string xero_generate_key_pair ( );


class BEXero : public BEOAuth {
	
public:
	
	explicit BEXero ( const std::string key, const std::string secret ) : BEOAuth ( key, secret ) { }
	
	int sign_url ( std::string& url, std::string& post_arguments, const std::string http_method );

};


#endif /* defined(__BaseElements__BE_Xero__) */
