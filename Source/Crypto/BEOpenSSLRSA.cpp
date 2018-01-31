/*
 BEOpenSSLRSA.cpp
 BaseElements Plug-In
 
 Copyright 2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEOpenSSLRSA.h"
#include "BEPluginException.h"


#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/pem.h>


using namespace std;


static const EVP_MD * getDigestByName ( const string &digestName )
{
	const EVP_MD * digest;
	
	if ( digestName.empty() ) {
		digest = EVP_sha256();
	} else {
		OpenSSL_add_all_algorithms();
		digest = EVP_get_digestbyname ( digestName.data() );
		if ( !digest ) {
			throw BEPlugin_Exception ( kRSATranslateDigestFailed );
		}
	}
	
	return digest;

}


static int passwordCallback ( char *buf, int max_len, int /* flag */, void *ctx )
{
	string &s = * static_cast<string *>(ctx);
	if ( s.length() > (size_t)max_len ) {
		return 0;
	}
	
	copy ( s.begin(), s.end(), buf ) ;
	
	return (int)s.length();

}


const vector<char> SignatureGenerate_RSA ( const vector<unsigned char> data, const string privateKey, const string digestName, string privateKeyPassword )
{
	unique_ptr<BIO, decltype(&BIO_free)> priKeyBio ( BIO_new_mem_buf ( privateKey.data(), (int)privateKey.size() ), &BIO_free );
	if ( !priKeyBio ) {
		throw BEPlugin_Exception ( kRSALoadBioFailed );
	}

	// Without this, passwordCallback is not invoked.
	OpenSSL_add_all_algorithms();
	unique_ptr<RSA, decltype(&RSA_free)> priKeyRsa ( PEM_read_bio_RSAPrivateKey ( priKeyBio.get(), NULL, passwordCallback, &privateKeyPassword ), &RSA_free );
	if ( !priKeyRsa ) {
		throw BEPlugin_Exception ( kRSAReadPrivateKeyFailed );
	}

	unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_destroy)> ctx ( EVP_MD_CTX_create(), &EVP_MD_CTX_destroy );
	unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> evpPkey ( EVP_PKEY_new(), &EVP_PKEY_free );

	if ( EVP_PKEY_set1_RSA( evpPkey.get(), priKeyRsa.get() ) <= 0 ) {
		throw BEPlugin_Exception ( kRSAReadPrivateKeyFailed );
	}

	const EVP_MD * digest = getDigestByName ( digestName );

	if ( EVP_SignInit_ex ( ctx.get(), digest, NULL ) <= 0 ) {
		throw BEPlugin_Exception ( kRSASignInitFailed );
	}

	if ( EVP_SignUpdate ( ctx.get(), data.data(), data.size() ) <= 0 ) {
		throw BEPlugin_Exception ( kRSASignUpdateFailed );
	}

	unsigned int outlen = EVP_PKEY_size ( evpPkey.get() );
	unique_ptr<unsigned char, decltype(&free)> out ( (unsigned char *)malloc ( outlen ), free );
	if ( EVP_SignFinal ( ctx.get(), out.get(), &outlen, evpPkey.get() ) <= 0 ) {
		throw BEPlugin_Exception ( kRSASignFinalFailed );
	}

	vector<char> result;
	result.insert ( result.end(), out.get(), out.get() + outlen );
	
	return result;

} // SignatureGenerate_RSA


const bool SignatureVerify_RSA ( const vector<unsigned char> data, const string publicKey, const vector<char> signature, const string digestName )
{
	unique_ptr<BIO, decltype(&BIO_free)> pubKeyBio ( BIO_new_mem_buf ( publicKey.data(), (int)publicKey.size() ), &BIO_free );
	if ( !pubKeyBio ) {
		throw BEPlugin_Exception ( kRSALoadBioFailed );
	}

	unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> evpPkey ( EVP_PKEY_new(), &EVP_PKEY_free );
	if ( !evpPkey ) {
		throw BEPlugin_Exception ( kLowMemoryError );
	}

	// Try to read the public key as PKCS#1
	unique_ptr<RSA, decltype(&RSA_free)> pkeyPkcs1 ( PEM_read_bio_RSAPublicKey ( pubKeyBio.get(), NULL, NULL, NULL ), &RSA_free );
	if ( pkeyPkcs1 ) {
		// It's PKCS#1. Convert it from RSA to EVP_PKEY.
		if ( EVP_PKEY_set1_RSA ( evpPkey.get(), pkeyPkcs1.get() ) <= 0 ) {
			throw BEPlugin_Exception ( kRSAReadPublicKeyFailed );
		}
	} else {
		BIO_reset ( pubKeyBio.get() );
		// Retry as PKCS#8
		evpPkey = unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> ( PEM_read_bio_PUBKEY ( pubKeyBio.get(), NULL, NULL, NULL ), &EVP_PKEY_free );
		if ( !evpPkey ) {
			// The public key is invalid or in unknown format.
			throw BEPlugin_Exception ( kRSAReadPublicKeyFailed );
		}
	}

	unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_destroy)> ctx ( EVP_MD_CTX_create(), &EVP_MD_CTX_destroy );

	const EVP_MD * digest = getDigestByName ( digestName );

	if ( EVP_VerifyInit_ex ( ctx.get(), digest, NULL ) <= 0 ) {
		throw BEPlugin_Exception ( kRSAVerifyInitFailed );
	}

	if ( EVP_VerifyUpdate ( ctx.get(), data.data(), data.size() ) <= 0 ) {
		throw BEPlugin_Exception ( kRSAVerifyUpdateFailed );
	}

	return EVP_VerifyFinal ( ctx.get(), (unsigned char *)signature.data(), (unsigned int)signature.size(), evpPkey.get() );

} // SignatureGenerate_RSA

