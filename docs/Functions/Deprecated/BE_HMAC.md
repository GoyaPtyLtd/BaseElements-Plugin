BE_HMAC

    BE_HMAC ( text ; key ; { algorithm ; outputEncoding ; inputEncoding } ) 

**Description**  

Does HMAC encoding on the *text*, using the *key* with a specific *algorithm* and *outputEncoding*, assuming a certain *inputEncoding* for the text and key.

**Parameters**

* *text* : the text to be encoded.
* *key* : the key to encode with.
* *algorithm* ( optional, default:BE_MessageDigestAlgorithm_SHA1 ) : Any of the BE_MessageDigestAlgorithm options.
* *outputEncoding* ( optional, default:BE_Encoding_Hex ) : *BE_Encoding_Hex* or *BE_Encoding_Base64*.
* *inputEncoding* ( optional, default:BE_Encoding_Hex ) : *BE_Encoding_Hex* or *BE_Encoding_Base64*.

**Keywords**  

Removed

**Version History**

* 3.0.0 : First Release
* 4.0.0 : Deprecated. Use internal *CryptAuthCode* instead.
* 4.2.0 : Removed.

