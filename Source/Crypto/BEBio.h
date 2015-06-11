/*
 BEBio.h
 BaseElements Plug-In

 Copyright 2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */

#ifndef __BaseElements__BEBio__
#define __BaseElements__BEBio__


#include "BEPluginGlobalDefines.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

#include <string>


class BEBio {


public:

	BEBio ( );
	~BEBio ( );

	const std::string extract ( X509 * x509 );
	const std::string extract ( RSA * rsa_key_pair );
	const std::string extract ( EVP_PKEY * private_key );

protected:

	BIO * bio;

	const std::string extract ( );

};


#endif /* defined(__BaseElements__BEBio__) */
