## BE_HTTP_SetProxy

    BE_HTTP_SetProxy ( proxy { ; port ; userName ; password } )

**Description**  

Used for setting the HTTP proxy values values before other curl based function calls.

**Parameters**

* *proxy* : The proxy url.
* *port* ( optional, default:80 ) : port number to use.
* *userName* ( optional ) : username for basic HTTP authentication.
* *password* ( optional ) : password for basic HTTP authentication.

**Keywords**  

keyword keyword

**Version History**

* 2.0.0 : First Release
* 4.0.2 : Renamed from BE_HTTP_Set_Proxy

**Notes**

There is no standard way to retrieve the built in OS proxy settings, if they've been applied, so this can only be set manually via this function.

Proxies are used in the HTTP, FTP, and SMTP functions despite the HTTP in the name of this function.

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

**Example Code**
