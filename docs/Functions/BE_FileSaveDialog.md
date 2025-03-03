## BE_FileSaveDialog

    BE_FileSaveDialog ( prompt ; { fileName ; inFolder } )

**Description**  

Displays the standard OS save file dialog. Changes the title of the dialog to the *prompt* specified, and defaults to *fileName* and sets the starting location as *inFolder*.

Returns the file path and filename that the user selected.  Returns an empty string when the user hits cancel or a path otherwise, but also check BE_GetLastError for non zero error codes.

**Parameters**

* *prompt* : The text to display in the dialog.
* *fileName* ( optional ) : The filename to start with. Leave empty to get the user to name the file.
* *inFolder* ( optional ) : The folder path to start in when opening the dialog. Defaults to the last used folder as determined by the operating system.

**Keywords**  

Dialog File Save

**Version History**

* 2.3.0 : First Release
* 4.0.0 : Return error 3, command not available on iOS, Linux and under FMS
* 4.0.2 : Renamed from BE_SaveFileDialog

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
