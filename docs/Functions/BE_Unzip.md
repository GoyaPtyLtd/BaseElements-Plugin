## BE_Unzip

    BE_Unzip ( archiveFilePath ; { outputFolderPath } )

**Description**  

Unzips the archive file found at the path *archiveFilePath*. The unzipped file(s) are put into the same location as the zip file itself by default, or into the *outputFolderPath* if specified. Any existing file(s) will be overwritten.

**Parameters**

* *archiveFilePath* : a plugin file path, or a container field.
* *filename* : a system folder path to put the result..

 If the *archiveFilePath* container field contains text, it's treated as a path to a file.
 
**Keywords**  

Zip Unzip

**Version History**

* 1.3.0 : First Release
* 2.2.0 : added the outputFolderPath parameter
* 4.2.0 : added the option to allow archiveFilePath to be a container field

**Notes**

If you're unzipping a file, then this function should be used with a *Set Field* script step and the results stored as a container field.  If you're expecting a text result then this could be used anywhere text is expected.
 
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

	BE_Unzip ( BE_SelectFile ( "Select a Zip file to unzip." ) )

	BE_Unzip ( "/Users/nick/Desktop/MyFile.zip" )
	BE_Unzip ( "/Users/nick/Desktop/MyFile.zip" ; "/Users/nick/Desktop/ResultFolder")
	
	