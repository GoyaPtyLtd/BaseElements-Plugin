## BE_CurlGetInfo

    BE_CurlGetInfo ( getInfoOption )

**Description**  

This function returns a value for the most recent curl operation ( HTTP, FTP, SMTP ) using the list of options that are supported by the BE_CurlSetOption function, and documented here :

https://curl.se/libcurl/c/curl_easy_getinfo.html

**Parameters**

* *getInfoOption* : Any of the values as a text string that are documented as working in the BE_CurlSetOption function.

**Keywords**  

curl option get set http ftp sftp smtp

**Version History**

* 5.0.0 : First Release

**Notes**

Not all options are supported in the plugin vs what is on the curl website, but the website documents them well to understand their usage.

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

    BE_CurlSetOption ( "CURLOPT_CERTINFO" ; 1 ) & 
    BE_HTTP_GET ( "https://goya.com.au" ) & 
    BE_CurlGetInfo ( "CURLINFO_CERTINFO" )
    
