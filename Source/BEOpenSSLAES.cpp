/*
 BEOpenSSLAES.cpp
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEOpenSSLAES.h"


#include "openssl/evp.h"


using namespace std;
using namespace fmx;


//		http://www.openssl.org/docs/crypto/EVP_EncryptInit.html#


const vector<unsigned char> Encrypt_AES ( const string key, const string text, const string input_vector )
{
	errcode error = NoError();
	
	vector<unsigned char> result;
//	result.clear();
	
	try {
		
			EVP_CIPHER_CTX context;
			EVP_CIPHER_CTX_init ( &context );
			int reply = EVP_EncryptInit_ex ( &context, EVP_aes_256_cfb128(), NULL, (unsigned char *)key.c_str(), (unsigned char *)input_vector.c_str() );
			
			if ( reply ) {
				
				const size_t input_size = text.size();
				const size_t output_size = input_size + EVP_MAX_BLOCK_LENGTH;
				unsigned char * encrypted_data = new unsigned char [ output_size ]();

				int output_length = 0;
				int final_output_length = 0;

				reply = EVP_EncryptUpdate ( &context, encrypted_data, &output_length, (unsigned char *)text.c_str(), (int)input_size );
				if ( reply ) {
					
					reply = EVP_EncryptFinal_ex ( &context, encrypted_data + output_length, &final_output_length );
					//			if ( reply ) {
					//			}
					
				}
				
				EVP_CIPHER_CTX_cleanup ( &context );
				
//				string prefix = input_vector + FILEMAKER_END_OF_LINE;
//				result.insert ( result.end(), prefix.begin(), prefix.end() );
				result.insert ( result.end(), encrypted_data, encrypted_data + output_length + final_output_length );
				
				if ( encrypted_data ) {
					delete[] encrypted_data;
				}
				
				
			}
		
	
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return result;
//	return MapError ( error );
	
} // Encrypt_AES


const vector<unsigned char> Decrypt_AES ( const string key, const vector<unsigned char> encrypted_data, const string input_vector )
{
	errcode error = NoError();
	
	vector<unsigned char> result;
//	result.clear();
	
	try {
		
		EVP_CIPHER_CTX context;
		EVP_CIPHER_CTX_init ( &context );
		int reply = EVP_DecryptInit_ex ( &context, EVP_aes_256_cfb128(), NULL, (unsigned char *)key.c_str(), (unsigned char *)input_vector.c_str() );
	
		if ( reply ) {
		
			const size_t input_size = encrypted_data.size();
			const size_t output_size = input_size + EVP_MAX_BLOCK_LENGTH;
			unsigned char * decrypted_data = new unsigned char [ output_size ]();

			int output_length = 0;
			int final_output_length = 0;
		
			reply = EVP_DecryptUpdate ( &context, decrypted_data, &output_length, (unsigned char *)&encrypted_data[0], (int)input_size );
			if ( reply ) {
			
				reply = EVP_DecryptFinal_ex ( &context, decrypted_data + output_length, &final_output_length );
				//			if ( reply ) {
				//			}
			
			}
		
			EVP_CIPHER_CTX_cleanup ( &context );
		
			result.insert ( result.end(), decrypted_data, decrypted_data + output_length + final_output_length );
		
			if ( decrypted_data ) {
				delete[] decrypted_data;
			}
					
		}
		
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return result;
	//	return MapError ( error );
	
} // Decrypt_AES


