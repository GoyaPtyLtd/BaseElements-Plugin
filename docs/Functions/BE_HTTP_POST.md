## BE_HTTP_POST

    BE_HTTP_POST ( url ; parameters ; { username ; password ; fileName } )

**Description**  

Does a HTTP POST call at the *url* and returns the response if any. This uses the same curl library as the built in *Insert From URL* using "-X POST" in the curl options.

**Parameters**

* *url* : The url to use for the POST action.
* *parameters* : The data to send to the url.
* *username* ( optional ) : The username if the url requires authentication.
* *password* ( optional ) : The password if the url requires authentication.
* *fileName* ( optional ) : The filename to send when sending binary data.

**Keywords**  

HTTP POST Curl

**Version History**

* 1.3.0 : First Release
* 2.0.0 : Added optional username and password parameters
* 3.1.0 : Allowed the use of file=@path for file parameters
* 4.0.0 : Added the optional filename parameter

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

When doing basic url parameter values, *parameters* is a text string, which is a list of all of the parameters to send in name value pair format. Put an "=" between the name and value, and an ampersand "&" between each pair. URLEncode the names and values.

Otherwise the type of data being sent is usually determined via a Content_Type header you set via *BE_HTTP_SetCustomHeader*. For example, JSON data : 

	BE_HTTP_POST ( "http://Fictional.Server.com/service.js" ;
	JSONSetElement ( "{}" ; "method" ; "Workgroup.projects.listActive" ; JSONString ) ;
	"Administrator" ; "password123" )

To send a file from disk, use file=@path for external files.  The path is a plugin path, not a FileMaker path.

	BE_HTTP_POST ( $url ; "image=@/Users/nick/Desktop/test.jpg" )

The server must support the sending of files.

You can also do multipart/form data :

	BE_HTTP_SetCustomHeader ( "Content-type"; "multipart/mixed" )
	
	BE_HTTP_POST ( $url ; "a=b&c=d&image=@" & BE_ExportFieldContents ( Table::File ) ; "Administrator" ; "password123" ; GetContainerAttribute ( Table::File ; "filename" ) )