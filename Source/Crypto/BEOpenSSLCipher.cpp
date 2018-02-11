/*
 BEOpenSSLCipher.cpp
 BaseElements Plug-In
 
 Copyright 2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEOpenSSLCipher.h"
#include "BEPluginException.h"


#include <openssl/evp.h>


using namespace std;


static const EVP_CIPHER * GetCipherByName ( const string &cipher_name )
{
	const EVP_CIPHER * cipher;
	
	if ( cipher_name.empty() ) {
		cipher = EVP_aes_256_cfb128();
	} else {
		OpenSSL_add_all_ciphers();
		cipher = EVP_get_cipherbyname( cipher_name.data() );
		if ( !cipher ) {
			throw BEPlugin_Exception ( kCipherTranslateCipherFailed );
		}
	}
	
	return cipher;
	
}


const vector<char> CipherEncrypt ( const string cipher_name, const vector<unsigned char> data, const vector<unsigned char> key, const vector<unsigned char> iv, const bool padding )
{
	unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx ( EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free );
	if ( EVP_EncryptInit_ex ( ctx.get(), GetCipherByName ( cipher_name ), NULL, key.data(), iv.data() ) < 1 ) {
		throw BEPlugin_Exception ( kCipherEncryptInitFailed );
	}
	
	EVP_CIPHER_CTX_set_padding ( ctx.get(), padding );
	
	const long max_outlen = data.size() + EVP_MAX_BLOCK_LENGTH;
	if ( max_outlen > INT_MAX ) {
		throw BEPlugin_Exception ( kCipherDataTooLarge );
	}
	unique_ptr<unsigned char, decltype(&free)> out ( (unsigned char *)malloc ( max_outlen ), free );
	if ( !out ) {
		throw BEPlugin_Exception ( kLowMemoryError );
	}
	long total_outlen = 0;
	int outlen;
	if ( EVP_EncryptUpdate ( ctx.get(), out.get(), &outlen, data.data(), (int)data.size() ) < 1 ) {
		throw BEPlugin_Exception ( kCipherEncryptUpdateFailed );
	}
	total_outlen += outlen;
	if ( EVP_EncryptFinal_ex ( ctx.get(), out.get() + total_outlen, &outlen ) < 1 ) {
		throw BEPlugin_Exception ( kCipherEncryptFinalFailed );
	}
	total_outlen += outlen;

	vector<char> result;
	result.insert ( result.end(), out.get(), out.get() + total_outlen );
	
	return result;
	
} // CipherEncrypt


const vector<char> CipherDecrypt ( const string cipher_name, const vector<unsigned char> data, const vector<unsigned char> key, const vector<unsigned char> iv, const bool padding )
{
	unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx ( EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free );
	if ( EVP_DecryptInit_ex ( ctx.get(), GetCipherByName ( cipher_name ), NULL, key.data(), iv.data() ) < 1 ) {
		throw BEPlugin_Exception ( kCipherDecryptInitFailed );
	}
	
	EVP_CIPHER_CTX_set_padding ( ctx.get(), padding );
	
	const long max_outlen = data.size() + EVP_MAX_BLOCK_LENGTH;
	if ( max_outlen > INT_MAX ) {
		throw BEPlugin_Exception ( kCipherDataTooLarge );
	}
	unique_ptr<unsigned char, decltype(&free)> out ( (unsigned char *)malloc ( max_outlen ), free );
	if ( !out ) {
		throw BEPlugin_Exception ( kLowMemoryError );
	}
	long total_outlen = 0;
	int outlen;
	if ( EVP_DecryptUpdate ( ctx.get(), out.get(), &outlen, data.data(), (int)data.size() ) < 1 ) {
		throw BEPlugin_Exception ( kCipherDecryptUpdateFailed );
	}
	total_outlen += outlen;
	if ( EVP_DecryptFinal_ex ( ctx.get(), out.get() + total_outlen, &outlen ) < 1 ) {
		throw BEPlugin_Exception ( kCipherDecryptFinalFailed );
	}
	total_outlen += outlen;
	
	vector<char> result;
	result.insert ( result.end(), out.get(), out.get() + total_outlen );
	
	return result;
	
} // CipherDecrypt


