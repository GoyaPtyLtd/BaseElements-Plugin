## BE_HTTP_PUTData

    BE_HTTP_PUTData ( url ; path ; { username ; password } )

**Description**  

Does a HTTP PATCH PUT at the *url* and returns the response if any. This uses the same curl library as the built in *Insert From URL* using "-X PUT" in the curl options.

**Parameters**

* *url* : The url to use for the PUT action.
* *path* : The path to read the file to send to the url.
* *username* ( optional ) : The username if the url requires authentication.
* *password* ( optional ) : The password if the url requires authentication.

**Keywords**  

HTTP PUT File Curl

**Version History**

* 2.0.0 : First Release
* 2.1.0 : Changed the name to BE_HTTP_PUT_DATA to reflect the actual use of the function parameters
* 4.0.2 : Renamed from BE_HTTP_PUTData

**Notes**

Headers can be set beforehand with *BE_HTTP_SetCustomHeader*.  Authentication types and other options can be set beforehand with the *BE_CurlSetOption* function.

Not all servers respond with data when doing curl operations so use *BE_GetLastError* after the function call to tell if the call was able to be made, and then *BE_HTTP_ResponseCode* to check for the appropriate response code, and *BE_HTTP_ResponseHeaders* to get the response headers to diagnose any issues.

Error codes that you get from the *BE_GetLastError* function after this function call comes from the curl library itself and not the plugin.  To find a specific error code use this documentation : 

[http://curl.haxx.se/libcurl/c/libcurl-errors.html](http://curl.haxx.se/libcurl/c/libcurl-errors.html)

Also use the *BE_CurlTrace* function to see the transcript of the connection to diagnose any other issues that may have come up and can't be determined from the error value alone.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | Yes  |  
| iOS | Yes  |  
| Linux | Yes  |  

This function has been superseded by the Insert From URL script step, and may be deprecated in a future release.  However the BE functions allow for a larger number of curl options, have a better response mechanism with the separate BE_HTTP_ResponseCode function, and have the advantage of not being a single script step.  Whether or not to deprecate the BE functions depends on end user needs.

**Example Code**

The type of data being sent is usually determined via a Content_Type header you set via *BE_HTTP_SetCustomHeader*. Each file type will have it's own Content-Type and the web service will determine acceptable types.  

	BE_HTTP_PUTData ( "http://Fictional.Server.com/service.js" ;
	"/path/to/file.txt" ;
	"Administrator" ; "password123" )
