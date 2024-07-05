## BE_FileModificationTimestamp

    BE_FileModificationTimestamp ( path )

**Description**  

Returns the OS file modification time of the file at *path*.

**Parameters**

* *path* : a system file path.

**Keywords**  

File Modification Timestamp Path

**Version History**

* 3.1.2 : First Release
* 4.0.2 : Renamed from BE_File_Modification_Timestamp

**Notes**

It is up to the Operating system as to what time it returns, and some OS versions may be more precise ( down to the millisecond ) and so the result could be different in across OS versions, or change in future OS versions.

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
