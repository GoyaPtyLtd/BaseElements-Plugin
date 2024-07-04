## BE_FileSaveDialog

    BE_FileSaveDialog ( prompt ; { fileName ; inFolder } )

**Description**  

Displays the standard OS save file dialog. Changes the title of the dialog to the *prompt* specified, and defaults to *fileName* and sets the starting location as *inFolder*.

Returns the file path and filename that the user selected.  Check BE_GetLastError = -1 for when the user hits cancel or a value other than 0 for any other error.

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

Please read the FAQ page about file paths.

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
