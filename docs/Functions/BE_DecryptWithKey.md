## BE_DecryptWithKey

    BE_DecryptWithKey ( encryptedText ; keyText )

**Description**  

Decrypts some text that was previously encrypted using the public RSA version of this key.

**Parameters**

* *encryptedText* : the encrypted text as base64.
* *keyText* : the key in RSA PRIVATE KEY format as text.

**Keywords**  

encrypt decrypt rsa publicKey privateKey base64

**Version History**

* 5.0.0 : First Release

**Notes**

Uses the same logic as openssl on the command line so should be compatible with that.  These keys can be used as access keys for SSH or converted to other formats.

If you have issues with the keys, check the line endings.  The DataViewer can mangle line endings in copy and pasted text, and line breaks in keys need to be Char ( 10 ).

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
