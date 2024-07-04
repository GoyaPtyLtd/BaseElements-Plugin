## BE_FTP_Delete

    BE_FTP_Delete ( url ; { username ; password } )

**Description**  

Deletes a file at a specific ftp or sftp url.

**Parameters**

* *url* : The url to connect to the server and the path to locate the file to delete.
* *username* ( optional ) : The username if the url requires authentication.
* *password* ( optional ) : The password if the url requires authentication.

The url is both the server connection detail as :

	protocol://server:port/
	
and the path the file to delete, eg :

	protocol://server:port/path/to/file/to/delete.txt

See the notes for more info about paths.

**Keywords**  

FTP Delete Curl

**Version History**

* 3.2.0 : First Release

**Notes about paths**

Our limited testing seems to be that when using the *BE_FTP_Delete* function, the path is a relative path, not an absolute path.  So when you login to the server, you often get put into a "home" folder, such that the current path at login time is :

	/home/nick

So to delete a file at the path /home/nick/folder/file.txt you will need to use the url as :

	sftp://example.com:2000/folder/file.txt
	
and leave out the current folder.  This is the opposite to the *BE_FTP_Upload* and *BE_FTP_UploadFile* functions where the path is absolute.  With an upload of the same file, you would use :

	sftp://example.com:2000/home/nick/folder/file.txt
	
To upload the same file as the delete example.

The simple way to check for paths and default folders is to use an FTP client with a graphical user interface which shows the current folder hierarchy.

This may be an artefact of the particular server we test with, or may be a default with curl, or may be because of the way that we've implemented the functions with curl, and so may be different from other FTP clients.

**Notes**

This function supports ftp, sftp and ftps.  The url prefix determines what type of connection it uses, see examples below.

Headers can be set beforehand with *BE_HTTP_SetCustomHeader*.  Authentication types and other options can be set beforehand with the *BE_CurlSetOption* function.

Different servers may or may not respond with any data on this function call, so use *BE_GetLastError* to determine if the call was able to be made or not, and then the other HTTP functions for more information 

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

Connecting to a standard FTP server to delete a file :

	BE_FTP_Delete ( "ftp://example.com/path/folder/file.txt" )

Connecting to an SFTP server : 

	BE_FTP_Delete ( "sftp://example.com:2000/path/folder/file.txt" ; "account" ; "password" )

