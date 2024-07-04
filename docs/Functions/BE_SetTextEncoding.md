## BE_SetTextEncoding

    BE_SetTextEncoding ( { encoding } )

**Description**  

Sets the text encoding format for any function that writes or reads text on disk, for example the *BE_WriteTextToFile* function.

**Parameters**

* *encoding* ( optional, default:UTF_8 ) : the encoding format to use. By leaving this parameter off, or setting it to empty you reset the encoding to its default.

**Keywords**  

Set Text Encoding

**Version History**

* 1.0.0 : First Release

**Notes**

This is using the iconv library so the full list of encoding options can be found at 

[http://www.gnu.org/software/libiconv/ ](http://www.gnu.org/software/libiconv/ ) 

or by typing "iconv -l" on the command line.

Note that if you've got content that does not match the encoding specified, the result may be an error or may be indeterminate depending on the library in use.  As a general rule we don't try to convert one set of text to another as the outcome may not be as the user intended.

If you're at all curious about encoding and why it's so complex, this is a good write up :

[https://tonsky.me/blog/unicode/](https://tonsky.me/blog/unicode/) 

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

To use UTF_16 which is the default encoding of the XML DDR : 

	BE_SetTextEncoding ( "UTF-16" )

To use ISO-8859-1 which is a common windows encoding.

	BE_SetTextEncoding ( "ISO-8859-1" )

To revert back to default UTF-8 :

	BE_SetTextEncoding
