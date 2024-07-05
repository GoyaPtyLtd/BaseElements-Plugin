## BE_FileMove

    BE_FileMove ( fromFilePath ; toFilePath {; replaceDestinationFile } )

**Description**  

Moves the file specified in the *fromFilePath* parameter, to the location in the *toFilePath* parameter optionally replacing according to *replaceDestinationFile*.

**Parameters**

* *fromFilePath* : a plugin path to a file.
* *toFilePath* : a plugin path destination.
* *replaceDestinationFile* ( optional, default:False ) : boolean to replace the file or not.

**Keywords**  

File Move Replace Path

**Version History**

* 1.1.0 : First Release
* 4.0.2 : Renamed from BE_MoveFile

**Notes**

On Mac OS X, the Move operation only works if the source and Destination are on the same volume. To move files across volumes, use a Copy and then Delete the original.

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
