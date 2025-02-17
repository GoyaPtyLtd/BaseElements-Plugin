## BE_SMTPServer

    BE_SMTPServer ( server ; { port ; username ; password ; keepOpen } )

**Description**  

Stores the SMTP connection details for future calls to the *BE_SMTPSend* function.  

**Parameters**

* *server* : A domain name or IP address of the SMTP server to connect to.
* *port* : the port number ( a required parameter, but can be an empty string ).
* *userName* ( optional ) : The userName if the url requires authentication.
* *password* ( optional ) : The password if the url requires authentication.
* *keepOpen* ( optional, default:False , ProOnly ) : Whether to keep the connection open for future sends.

To use the *keepOpen* option, set this to True when sending multiple messages via SMTP in a loop to avoid closing the connection to the server and re-opening it every time.  This will be faster and less work for the server and the plugin.  To close the connection, use the same command but change the keepOpen parameter to False.

**Keywords**  

keyword keyword

**Version History**

* 3.1.0 : First Release
* 4.0.2 : Renamed from BE_SMTP_Server
* 5.0.0 : Renamed from BE_SMTPSend

**Notes**

This function doesn't connect to the server itself it only stores the connection details for future *BE_SMTPSend* function calls.

Authentication is controlled with curl, so additional options, other than the defaults can be set with the *BE_CurlSetOption* function, and headers can be set via the *BE_SMTPSetHeader* function.

Any future calls to this function overwrite any existing details.

port number can be left as an empty string, which will attempt to use whatever ports the server requires, default for SMTP is 25, but using SSL or TLS may use 465 or 587.

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
