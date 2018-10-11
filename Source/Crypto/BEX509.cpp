/*
 BEX509.cpp
 BaseElements Plug-In

 Copyright 2015-2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEX509.h"
#include "BEBio.h"
#include "BEPluginException.h"

#include <openssl/err.h>
#include <openssl/pem.h>

#include <vector>


BEX509::BEX509 ( )
{
	x509 = X509_new();

	if ( ! x509 ) {
		throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
	}

	if ( ! ASN1_INTEGER_set ( X509_get_serialNumber ( x509 ), 1 ) ) {
		throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
	}

	X509_gmtime_adj ( X509_get_notBefore ( x509 ), 0 ); // valid from now
	X509_gmtime_adj ( X509_get_notAfter ( x509 ), 157680000L ); // valid for 5 years

	name = X509_get_subject_name ( x509 );

	if ( ! name ) {
		throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
	}

}



BEX509::~BEX509 ( )
{
	X509_free ( x509 );
}


void BEX509::set_private_key ( EVP_PKEY * key )
{
	private_key = key;
	if ( ! X509_set_pubkey ( x509, private_key ) ) {
		throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
	}
};


void BEX509::add_name_entry ( const std::string entry, const std::string value )
{
	if ( ! entry.empty() && ! value.empty() ) {
		if ( ! X509_NAME_add_entry_by_txt ( name, entry.c_str(), MBSTRING_UTF8, (unsigned char *)value.c_str(), -1, -1, 0 ) ) {
			throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
		}
	}

}


const std::string BEX509::generate ( )
{
	if ( ! X509_set_issuer_name ( x509, name ) ) {
		throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
	}

	if ( ! X509_sign ( x509, private_key, EVP_sha1() ) ) {
		throw BEPlugin_Exception ( (fmx::errcode)ERR_get_error() );
	}

	std::unique_ptr<BEBio> bio ( new BEBio );

	return bio->extract ( x509 );
	
}


