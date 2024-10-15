## BE_GetPublicKey

    BE_GetPublicKey ( privateKey )

**Description**  

Generates the public key from a given **privateKey** generated with the BE_CreateKeyPair function.

**Parameters**

* *privateKey* : the RSA Private Key in text format.

**Keywords**  

RSA PrivateKey PublicKey

**Version History**

* 5.0.0 : First Release

**Notes**

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
