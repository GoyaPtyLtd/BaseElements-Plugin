/*
 BEX509.h
 BaseElements Plug-In

 Copyright 2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#ifndef __BaseElements__BEX509__
#define __BaseElements__BEX509__


#include "BEPluginGlobalDefines.h"

#include <openssl/evp.h>
#include <openssl/x509.h>

#include <string>


class BEX509 {


public:

	BEX509 ( );
	~BEX509 ( );

	void add_name_entry ( const std::string entry, const std::string value );
	const std::string generate ( );
	void set_private_key ( EVP_PKEY * key );
	
protected:

	X509 * x509;
	X509_NAME * name;

	EVP_PKEY * private_key;

};



#endif /* defined(__BaseElements__BEX509__) */
