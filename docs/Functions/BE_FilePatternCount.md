## BE_FilePatternCount

    BE_FilePatternCount ( path ; searchText )

**Description**  

Search inside files on disk. Much like the native *PatternCount* function, but within files on disk.

**Parameters**

* *path* : the plugin path to the file.
* *searchText* : the text to find in the text document - can be a value list, where it will count each of the values in turn.

**Keywords**  

File Pattern Count Path

**Version History**

* 4.2.0 : First Release

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
