## BE_SignatureGenerateRSA

    BE_SignatureGenerateRSA ( data ; privateKey ; { privateKeyPassword ; algorithm ; fileNameWithExtension } )

**Description**  

Generates a digital signature of *data*. The message digest of data is calculated first using the specified *algorithm*. Then the digest is encrypted with *privateKey*.

**Parameters**

* *data* : The data to be signed. It can be text or container field.
* *privateKey* : The openssl private key (as text) to digitally sign the input text.   PKCS#1 and PKCS#8 PEM formats are supported.
* *privateKeyPassword* ( optional, default:empty ) : The password of the private key if required. It will be ignored if the private key is not password-protected.
* *algorithm* ( optional, default:SHA256 ) : The hash algorithm name (as text) use to calculate the digest of the data.
* *fileNameWithExtension* ( optional, default:BE_MessageDigestAlgorithmSHA256 ) : The filename and extension for the generated signature file.

**Keywords**  

Signature RSA

**Version History**

* 4.0.0 : First Release
* 4.0.2 : Renamed from BE_SignatureGenerate_RSA

**Notes**

Uses OpenSSL so algorithms come from there, and should be generally compatible with the equivalent openssl commands.

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
