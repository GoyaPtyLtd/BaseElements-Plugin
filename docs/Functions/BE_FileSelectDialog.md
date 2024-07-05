## BE_FileSelectDialog

    BE_FileSelectDialog ( prompt { ; inFolderPath } )

**Description**  

Displays the standard OS select file dialog with the title of the dialog to the *prompt* specified, and starting path of the *inFolderPath*.

Result is the path to the file selected by the user.  Check BE_GetLastError = -1 for when the user hits cancel or a value other than 0 for any other error.

**Parameters**

* *prompt* : The text to display in the dialog.
* *nainFolderPathme* ( optional ) : The folder to start in when opening the dialog. Defaults to the last used folder as determined by the operating system.

**Keywords**  

File Select Dialog

**Version History**

* 1.0.0 : First Release
* 2.0.0 : Added the optional inFolderPath parameter.
* 2.2.0 : Added the ability to select multiple files.
* 2.2.0 : Update dialogs for new Windows OS versions
* 4.0.0 : Return error 3, command not available on iOS, Linux and under FMS
* 4.0.2 : Renamed from BE_SelectFile

**Notes**

PLUGIN PATHS ARE NOT FILEMAKER PATHS. The plugin uses the same path structure as your operating system, and you cannot pass to the plugin paths that start with file:/ or filewin:/ etc.  Read the FAQ for more info about paths.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | No  |  
| iOS | No  |  
| Linux | No  |  

**Example Code**
