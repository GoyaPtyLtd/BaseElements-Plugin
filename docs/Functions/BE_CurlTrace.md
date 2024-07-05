## BE_CurlTrace

    BE_CurlTrace
	
**Description**  

Returns a complete trace of the HTTP, FTP, and SMTP details for the most recent function call.  This is useful for debugging connection issues.

**Parameters**

None

**Keywords**  

HTTP FTP SMTP Curl Trace

**Version History**

* 3.1.0 : First Release
* 4.0.2 : Renamed from BE_Curl_Trace

**Notes**

This is to get the HTTP, FTP or SMTP transcript of the connection so that you can more easily diagnose connections issues, especially for encrypted connections ( https or SMTP with SSL ) where you can't listen in to the connection.

This behaves the same as the Error functions in that it only remembers the last HTTP/SMTP/FTP call.

This functionality is enabled by default and can be disabled by setting the Curl Option CURLOPT_VERBOSE.

	BE_CurlSetOption ( "CURLOPT_VERBOSE" ; 0 )

Error codes encountered during curl operations come from the curl library itself and not the plugin.  To find a specific error code use this documentation : 

[http://curl.haxx.se/libcurl/c/libcurl-errors.html](http://curl.haxx.se/libcurl/c/libcurl-errors.html)

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
