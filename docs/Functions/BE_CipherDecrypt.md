## BE_CipherDecrypt

    BE_CipherDecrypt ( cipher ; encryptedData ; key ; iv ; { paddingBoolean ; fileNameWithExtension } )

**Description**  

Decrypt *encryptedData* using *cipher*.

**Parameters**

* *cipher* ( default:AES-256-CBC ) : The cipher name to use. If the value is empty, "AES-256-CBC" will be used.
* *encryptedData* : The data to decrypt. It can be HEX encoded text or container field.
* *key* : The key to decrypt with. It can be HEX encoded text or container field.
* *iv* : The IV (initialization vector). It can be HEX encoded text or container field.
* *paddingBoolean* ( optional, default:True ) : Set false to disable padding. Padding is enabled if the parameter is true, empty or not specified.
* *fileNameWithExtension* : The filename and extension for the decrypted data if being set into a container field.

**Keywords**  

Cipher Decrypt

**Version History**

* 4.0.0 : First Release

**Notes**

Modes that require authenticated encryption (e.g. GCM) are not supported. These ciphers return 'authentication tag' in addition to encrypted data.

The max data length is limited to 2GB.

The code uses OpenSSL for it's base library, so the list of ciphers and their details are here : https://www.openssl.org/docs/man1.0.2/man1/ciphers.html however not all available ciphers may work, and this list may change with future releases of openssl.

Not all possible ciphers will work, and thorough testing on Encryption and Decryption should be done before putting code into production.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | Yes  |  
| iOS | Yes  |  
| Linux | Yes  |  

**Example Code**
