## BE_SignatureVerifyRSA

    BE_SignatureVerifyRSA ( data ; publicKey ; { signature ; algorithm )

**Description**  

Verifies if signature is valid for *data* by comparing the message digest of *data* with the digest obtained from *signature* by decrypting it using *publicKey*.

**Parameters**

* *data* : The source data that was signed. It can be text or container field.
* *publicKey* : The openssl public key (as text) to verify the digital signature. PKCS#1 and PKCS#8 PEM formats are supported.
* *signature* : The digital signature. It can be container field or Base64 encoded text.
* *algorithm* ( optional, default:SHA256 ) : The hash algorithm name (as text) use to calculate the digest of the data.

**Keywords**  

Signature RSA Verify

**Version History**

* 4.0.0 : First Release
* 4.0.2 : Renamed from BE_SignatureVerify_RSA

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
