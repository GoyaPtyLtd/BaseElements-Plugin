## BE_DialogDisplay

    BE_DialogDisplay ( title ; message ; defaultButton ; { cancelButton ; alternateButton } )

**Description**  

Puts up a dialog almost exactly the same as the one that you can get with the Show Custom Dialog script step, minus the fields.  Use this function when you want to display a dialog from a calculation instead of a script step.

Result is the number of the button clicked - use the Constants as text replacements for button numbers.

**Parameters**

* *title* : The dialog title.
* *message* : The content of the dialog.
* *defaultButton* : Right most button text.
* *cancelButton* : Left most button text.
* *alternateButton* : Alternate button text.

**Keywords**  

Dialog Display

**Version History**

* 1.0.0 : First Release
* 4.0.2 : Renamed from BE_DisplayDialog

**Notes**

The original intent of this function was that the built in step didn't allow you to customise some parts of the dialog, so this was critical for things like translated solutions.  With newer changes to FileMaker's native script step, this isn't as necessary however as a function imstead of a script step you can have other options to display dialogs.

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
