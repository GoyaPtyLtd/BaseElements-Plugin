## BE_HTTP_ResponseHeaders

    BE_HTTP_ResponseHeaders ( { header } )

**Description**  

Returns the headers set by the server as part of a response to the previous HTTP request made via any of the BE curl functions.

**Parameters**

* *header* ( optional ) : The header name to retrieve or all headers if blank or omitted.

**Keywords**  

HTTP Response Headers Curl

**Version History**

* 1.3.0 : First Release
* 3.3.2 : Added the *header* parameter so that the named header only is returned
* 4.0.2 : Renamed BE_HTTP_Response_Code to BE_HTTP_ResponseCode

**Notes**

Notes

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
