## BE_FileCopy

    BE_FileCopy ( fromFilePath ; toFilePath {; replaceDestinationFile } )

**Description**  

Copies the file at *fromFilePath* to *toFilePath*, optionally replacing the destination file when *replaceDestinationFile* is set to True.   

**Parameters**

* *fromFilePath* : a path to a file.
* *toFilePath* : a destination file path, including filename.
* *replaceDestinationFile* ( optional ) : whether to overwrite an existing file, default is false.

**Keywords**  

File Copy Path

**Version History**

* 1.1.0 : First Release
* 1.2.0 : Added support for copying directories
* 4.0.2 : Renamed from BE_CopyFiles

**Notes**

All paths should be absolute paths, there is no defined *current* folder, so relative paths may or may not work.

Important to note that the *toFilePath* path needs to end with the desired name of the file, and that that it doesn't need to match the *fromFilePath* filename.  So this function can be used to rename files by doing a from/to with the same path but different filename.

*PLUGIN PATHS ARE NOT FILEMAKER PATHS* The plugin uses the same path structure as your operating system, and you cannot use paths that start with file:/ or filewin:/ etc.  Read the FAQ for more info about paths.

This function does not behave the same as /bin/cp.

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