## BE_FTP_UploadFile

    BE_FTP_UploadFile ( url ; pathToFile ; { username ; password } )

**Description**  

Uploads the to the ftp/sftp/ftps *url* specified, using the *pathToFile* as the location of the file to upload from disk.

**Parameters**

* *url* : The url to connect to the server, with the path to locate the file to upload, and the filename to upload as.
* *pathToFile* : A plugin path to the file on disk.
* *username* ( optional ) : The username if the url requires authentication.
* *password* ( optional ) : The password if the url requires authentication.

The url is both the server connection detail as :

	protocol://server:port/
	
and the path is both the location of the file to upload and the name of the file to upload, eg :

	protocol://server:port/path/to/file/to/upload.txt

See the notes for more info about paths.

**Keywords**  

FTP Upload File Path Curl

**Version History**

* 4.2.0 : First Release

**Notes about paths**

Our limited testing seems to be that when using the *BE_FTP_UploadFile* function, the destination path is an absolute path, not an relative path to the users default.  So when you login to the server, you often get put into a "home" folder, such that the current path at login time is :

	/home/nick

So to upload a file at the path /home/nick/folder/file.txt you will need to use the url as :

	sftp://example.com:2000/home/nick/folder/file.txt
	
So you need to use the full path to the location.  Also note that you can't often get outside your own home folder, so any other path would fail.  This is the opposite to the *BE_FTP_Delete* and function where the path is a relative path.  So to delete the same file you uploaded above, you would use :

	sftp://example.com:2000/folder/file.txt
	
The simple way to check for paths and default folders is to use an FTP client with a graphical user interface which shows the current folder hierarchy.

This may be an artefact of the particular server we test with, or may be a default with curl, or may be because of the way that we've implemented the functions with curl, and so may be different from other FTP clients.

**Notes**

This function supports ftp, sftp and ftps.  The url prefix determines what type of connection it uses, see examples below.

Headers can be set beforehand with *BE_HTTP_SetCustomHeader*.  Authentication types and other options can be set beforehand with the *BE_CurlSetOption* function.

Different servers may or may not respond with any data on this function call, so use *BE_GetLastError* to determine if the call was able to be made or not, and then the other HTTP functions for more information afterwards.

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

	BE_FTP_UploadFile ( "ftp://example.com/path/folder/myfilname.txt" ; "/Volumes/HD/Path/To/myfilname.txt" )

	BE_FTP_UploadFile ( "sftp://example.com:2000/path/folder/myfilname.txt" ; "/Volumes/HD/Path/To/myfilname.txt" ; "account" ; "password" )
