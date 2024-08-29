## BE_FileDelete

    BE_FileDelete ( path )

**Description**  

Deletes the file at location path. 

**Parameters**

* *path* : a plugin file path.

**Keywords**  

File Delete Path

**Version History**

* 1.0.0 : First Release
* 4.0.2 : Renamed from BE_DeleteFile

**Notes**

This function doesn't send files to the trash or recycle bin, so use with caution. Files deleted are gone.

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

	BE_FileDelete ( "/Users/username/Desktop/myNewFolder" )

deletes a folder and all of it's contents

	BE_FileDelete ( "/Users/username/Desktop/fileA.fp7" )

deletes a file
