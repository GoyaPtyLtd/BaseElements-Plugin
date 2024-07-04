## BE_FileCopy

    BE_FileCopy ( fromFilePath ; toFilePath {; replaceDestinationFile } )

**Description**  

Deletes the file at location path. 

**Parameters**

* *fromFilePath* : a plugin file path to copy from.
* *toFilePath* : a plugin file path destination, including filename.
* *replaceDestinationFile* : whether to overwrite an existing file.

**Keywords**  

File Copy Path

**Version History**

* 1.1.0 : First Release
* 1.2.0 : Added support for copying directories
* 4.0.2 : Renamed from BE_CopyFiles

**Notes**

This function does not behave the same as /bin/cp.

Important to note that the 'to' path needs to end with the desired name of the file.

PLUGIN PATHS ARE NOT FILEMAKER PATHS. The plugin uses the same path structure as your operating system, and you cannot pass to the plugin paths that start with file:/ or filewin:/ etc.  Read the FAQ for more info about paths.

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

	BE_FileCopy ( "/Users/username/Desktop/fileA.fp7" ; "/Users/username/path/to/file/fileA_copy.fp7" )