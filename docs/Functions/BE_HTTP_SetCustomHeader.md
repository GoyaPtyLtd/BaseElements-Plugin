## BE_HTTP_SetCustomHeader

    BE_HTTP_SetCustomHeader ( { header ; value } )

**Description**  

Used for setting a header value before a HTTP, FTP, or SMTP function call. You can call this function multiple times before the POST to set more than one header, and call it with empty parameters to clear them out.

**Parameters**

* *header* ( optional ) : the name of the header to set.
* *value* ( optional ) : the value to set it to.

**Keywords**  

HTTP Header Set Custom Curl

**Version History**

* 1.3.0 : First Release
* 4.0.1 : Added option to call with no parameters to delete all Headers
* 4.0.2 : Renamed from BE_HTTP_Set_Custom_Header
* 4.1.3 : **Breaking Change** : Change the way empty string headers vs missing value parameters are used - see notes.

**Notes**

In versions from 4.1.3 or later, setting a header with 

	BE_HTTP_SetCustomHeader ( "name" ; "" ) 

now sets the header to an empty header value, instead of removing that header. Some header options have a default value and there is an accepted header with the name, but an empty string as the value.  To remove the header completely and revert to default call 

	BE_HTTP_SetCustomHeader ( "name" ) 

with no value parameter, which is also the same behaviour as the previous versions.

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

	BE_HTTP_SetCustomHeader ( "Content-Type" ; "text/xml;charset=utf-8" ) 
