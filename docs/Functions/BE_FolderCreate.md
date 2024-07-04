## BE_FolderCreate

    BE_FolderCreate ( path )

**Description**  

Creates a new folder at the location *path*, creating sub folders along the way as required.

**Parameters**

* *path* : A plugin file path ( not a FileMaker path )..

**Keywords**  

Path Create

**Version History**

* 1.0.0 : First Release
* 2.1.0 : changed the folder to be recursive, so any required subfolders are also created
* 4.0.2 : Renamed from BE_CreateFolder

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

	BE_FolderCreate ( "/Users/username/Desktop/myNewFolder" )
