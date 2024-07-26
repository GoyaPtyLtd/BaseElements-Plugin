## BE_FileListFolder

    BE_FileListFolder ( path { ; type ; includeSubdirBoolean ; useFullPathBoolean ; includeHiddenBoolean } )

**Description**  

Lists the contents of a folder at the *path*, both files and folders by default or use a *type* from *BE_FileTypeAll*, *BE_FileTypeFile*, or *BE_FileTypeFolder*.

**Parameters**

* *path* : Operating System path to start from.
* *type* ( optional, default:BE_FileTypeAll ) : one of BE_FileTypeAll, BE_FileTypeFile, BE_FileTypeFolder.
* *includeSubdirBoolean* ( optional, default:False ) : whether to scan sub directories.
* *useFullPathBoolean* ( optional, default:False ) : whether to include the full path ( True ) or just filenames ( False ) in the output.
* *includeHiddenBoolean* ( optional, default:False ) : whether to include files or folder not normally visible.

**Keywords**  

File Folder List Path

**Version History**

* 1.1.0 : First Release
* 1.2.0 : Added the optional type parameter
* 2.3.0 : Added the optional includeSubdirectories and useFullPath parameters
* 4.0.0 : Added the optional includeHidden parameter
* 4.1.3 : Correctly handle file names containing Unicode Characters on Windows

**Notes**

The *includeSubdirectories* option means that it will try every single subfolder. Be cautious when using this as it may take a long time to traverse all the sub folders.  

Also it is more than likely that at some point it will throw an error as it will come across a folder or file it doesn't have access to. Then the function will stop and return error 13, and no data. Managing individual errors like that amongst a potentially large set of files is beyond the scope of this function as implemented.

If you're getting error 13 when using this flag, consider doing without it and traversing the sub folders via script or recursion and ignoring the access error codes instead.

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

	BE_FileListFolder ( BE_FolderSelectDialog ( "" ) )

	BE_FileListFolder ( "/Users/nick/Desktop" )

	BE_FileListFolder ( $path ; BE_FileTypeFolder ; False ; True ; True )
	
This last one will start at $path, but only return folders, and will not include subdirectories, will return a full path not just the folder names, and will include any hidden folders. :

	BE_FileListFolder ( $path ; BE_FileTypeFolder ;
	False ; //don't scan sub folders
	True ; //include a full path
	True ) //include hidden files

