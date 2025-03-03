## BE_FolderSelectDialog

    BE_FolderSelectDialog ( prompt { ; inFolderPath } )

**Description**  

Displays the standard OS select folder dialog with the title of the dialog to the *prompt* specified, and starting path of the *inFolderPath*.

Result is the path to the folder selected by the user.  Returns an empty string when the user hits cancel or a path otherwise, but also check BE_GetLastError for non zero error codes.

**Parameters**

* *prompt* : The text to display in the dialog.
* *nainFolderPathme* ( optional ) : The folder to start in when opening the dialog. Defaults to the last used folder as determined by the operating system.

**Keywords**  

File Select Dialog

**Version History**

* 1.0.0 : First Release
* 1.3.0 : Added the optional inFolderPath parameter.
* 2.0.0 : Add New Folder button.
* 2.2.0 : Update dialogs for new Windows OS versions
* 4.0.0 : Return error 3, command not available on iOS, Linux and under FMS
* 4.0.2 : Renamed from BE_SelectFolder

**Notes**

Notes

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
