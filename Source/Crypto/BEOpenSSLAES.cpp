/*
 BEOpenSSLAES.cpp
 BaseElements Plug-In
 
 Copyright 2014-2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEOpenSSLAES.h"
#include "BEPluginException.h"

#include <boost/algorithm/hex.hpp>
#include <openssl/evp.h>


using namespace std;


const vector<unsigned char> HexOrContainer ( const fmx::DataVect& parameters, const fmx::uint32 which )
{
	vector<unsigned char> output;
	
	if ( BinaryDataAvailable ( parameters, which ) ) {
		output = ParameterAsVectorUnsignedChar ( parameters, which );
	} else {
		auto str = ParameterAsUTF8String ( parameters, which );
		boost::algorithm::unhex ( str.begin(), str.end(), back_inserter ( output ) );
	}
	
	return output;
	
} // HexOrContainer


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


void GenerateKeyAndInputVector ( const string password, string& key, vector<char>& input_vector )
{
	const int key_length = 32;
	const int rounds = 5;
	unsigned char key_buffer [ key_length ];
	unsigned char iv [ key_length ];
	
	int key_size = EVP_BytesToKey ( EVP_aes_256_cbc(), EVP_sha1(), NULL, (unsigned char*)password.c_str(), (int)password.size(), rounds, key_buffer , iv );
	if ( key_size == key_length ) {
		
		key.assign ( key_buffer, key_buffer + key_length );
		input_vector.assign ( iv, iv + key_length );
		
	} else {
		throw BEPlugin_Exception ( kKeyEncodingError );
	}
	
} // GenerateKeyAndInputVector


//		http://www.openssl.org/docs/crypto/EVP_EncryptInit.html#


const vector<char> Encrypt_AES ( const string key, const string text, const vector<char> input_vector )
{
	vector<char> result;
	
	EVP_CIPHER_CTX context;
	EVP_CIPHER_CTX_init ( &context );
	int reply = EVP_EncryptInit_ex ( &context, EVP_aes_256_cfb128(), NULL, (unsigned char *)key.c_str(), (unsigned char *)&input_vector[0] );
	
	if ( reply ) {
		
		const size_t input_size = text.size();
		const size_t output_size = input_size + EVP_MAX_BLOCK_LENGTH;
		unsigned char * encrypted_data = new unsigned char [ output_size ]();
		
		int output_length = 0;
		int final_output_length = 0;
		
		reply = EVP_EncryptUpdate ( &context, encrypted_data, &output_length, (unsigned char *)text.c_str(), (int)input_size );
		if ( reply ) {
			EVP_EncryptFinal_ex ( &context, encrypted_data + output_length, &final_output_length ); // reply =			
		} else {
			throw BEPlugin_Exception ( kEncryptionUpdateFailed );
		}
		
		EVP_CIPHER_CTX_cleanup ( &context );
		
		result.insert ( result.end(), encrypted_data, encrypted_data + output_length + final_output_length );
		
		delete[] encrypted_data;
		
	} else {
		throw BEPlugin_Exception ( kEncryptionInitialisationFailed );
	}
	
	return result;
	
} // Encrypt_AES


const vector<char> Decrypt_AES ( const string key, const vector<char> encrypted_data, const vector<char> input_vector )
{
	vector<char> result;
	
	if ( !encrypted_data.empty() ) {
		
		EVP_CIPHER_CTX context;
		EVP_CIPHER_CTX_init ( &context );
		int reply = EVP_DecryptInit_ex ( &context, EVP_aes_256_cfb128(), NULL, (unsigned char *)key.c_str(), (unsigned char *)&input_vector[0] );
		
		if ( reply ) {
			
			const size_t input_size = encrypted_data.size();
			const size_t output_size = input_size + EVP_MAX_BLOCK_LENGTH;
			unsigned char * decrypted_data = new unsigned char [ output_size ]();
			
			int output_length = 0;
			int final_output_length = 0;
			
			reply = EVP_DecryptUpdate ( &context, decrypted_data, &output_length, (unsigned char *)&encrypted_data[0], (int)input_size );
			if ( reply ) {
				
				EVP_DecryptFinal_ex ( &context, decrypted_data + output_length, &final_output_length );
				
			} else {
				throw BEPlugin_Exception ( kDecryptionUpdateFailed );
			}
			
			EVP_CIPHER_CTX_cleanup ( &context );
			
			result.insert ( result.end(), decrypted_data, decrypted_data + output_length + final_output_length );
			
			delete[] decrypted_data;
			
		} else {
			throw BEPlugin_Exception ( kEncryptionInitialisationFailed );
		}
		
	}
	
	return result;
	
} // Decrypt_AES


//TODO The funtion pair CipherEncrypt/CipherDecrypt are substantially similar to the AES pair (above) and should be refactored/replaced by them


const vector<char> CipherEncrypt ( const string cipher_name, const vector<unsigned char> data, const vector<unsigned char> key, const vector<unsigned char> iv, const bool padding )
{
	const EVP_CIPHER * cipher = GetCipherByName ( cipher_name );
	
	const int expected_iv_size = EVP_CIPHER_iv_length ( cipher );
	if ( expected_iv_size > 0 && (unsigned long)expected_iv_size != iv.size() ) {
		throw BEPlugin_Exception ( kCipherInvalidIvSize );
	}
	
	unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx ( EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free );
	if ( EVP_EncryptInit_ex ( ctx.get(), cipher, NULL, key.data(), iv.data() ) < 1 ) {
		throw BEPlugin_Exception ( kCipherEncryptInitFailed );
	}
	
	EVP_CIPHER_CTX_set_padding ( ctx.get(), padding );
	
	if ( EVP_CIPHER_key_length ( cipher ) > 0 && EVP_CIPHER_CTX_set_key_length ( ctx.get(), (int)key.size() ) < 1 ) {
		throw BEPlugin_Exception ( kCipherInvalidKeySize );
	}
	
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
	const EVP_CIPHER * cipher = GetCipherByName ( cipher_name );
	
	const int expected_iv_size = EVP_CIPHER_iv_length ( cipher );
	if ( expected_iv_size > 0 && (unsigned long)expected_iv_size != iv.size() ) {
		throw BEPlugin_Exception ( kCipherInvalidIvSize );
	}
	
	unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx ( EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free );
	if ( EVP_DecryptInit_ex ( ctx.get(), cipher, NULL, key.data(), iv.data() ) < 1 ) {
		throw BEPlugin_Exception ( kCipherDecryptInitFailed );
	}
	
	EVP_CIPHER_CTX_set_padding ( ctx.get(), padding );
	
	if ( EVP_CIPHER_key_length ( cipher ) > 0 && EVP_CIPHER_CTX_set_key_length ( ctx.get(), (int)key.size() ) < 1 ) {
		throw BEPlugin_Exception ( kCipherInvalidKeySize );
	}
	
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

