## BE_Decrypt_AES

    BE_Decrypt_AES ( key ; text )

**Description**  

Decrypts the value in *text* encrypted with the BE_Encrypt_AES function and using the supplied *key*.

Result is the original unencrypted text or ? for errors.  Use the BE_GetLastError function to check for a successful result.

**Parameters**

* *key* : The secret key used in the BE_Encrypt_AES function when the text was encrypted.
* *text* : The text to be encrypted.

**Keywords**  

Decrypt AES

**Version History**

* 2.3.0 : First Release
* 4.0.0 : Deprecated with a recommendation to use the FileMaker 16 function *CryptDecrypt* ( data ; key ) instead.
* 4.1.0 : Removed the deprecation.

**Notes**

We removed the deprecation as the native *CryptDecrypt* function doesn't quite do all the same things that our function does, so there is still room for both.  We do recommend users use the native function where it's suitable.

The intention of this is purely that anything encrypted by the plugin can only be decrypted by the plugin and there is no guarantee that the methods and code used is the same as any other external AES encryption.

Although it is based on open standards, and you may find that you can encrypt/decrypt with software other than the BE plugin, it isn't guaranteed.

Specific external compatibility testing would be useful to add.

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
