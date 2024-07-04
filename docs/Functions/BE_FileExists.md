## BE_FileExists

    BE_FileExists ( path )

**Description**  

Checks if the file or folder given in the *path* exists and returns True or False.  For folders it accepts the name of the folder ( and reports existence correctly ) with or without a trailing /.

**Parameters**

* *path* : a plugin file path.

**Keywords**  

File Exists Path

**Version History**

* 1.0.0 : First Release

**Notes**

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
