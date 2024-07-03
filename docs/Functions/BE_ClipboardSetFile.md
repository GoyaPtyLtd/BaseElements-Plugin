## BE_ClipboardSetFile

    BE_ClipboardSetFile ( fileData ; format )

**Description**  

Sets the clipboard to the *fileData* and applies a clipboard *format*. Clipboard formats are set by the application that does the copy and vary widely based on the content types.

Use the *BE_ClipboardFormats* function to get examples of different types. The *format* you choose should depend on what you expect the user to do with the clipboard content.

This function only works with binary data, to set text data use *BE_ClipboardSetText*.

**Parameters**

* *fileData* : A container field or a file set into a variable.
* *format* : The format text code for the content.

**Keywords**  

Clipboard Data Set

**Version History**

* 4.0.3 : First Release

**Notes**

When setting a type it's best to copy a sample of that type from some other application and then use *BE_ClipboardFormats* to see what types are used.

You can only set a single format when setting the clipboard from BE, even though the clipboard allows multiple types.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | Yes  |  
| iOS | No  |  
| Linux | No  |  

**Example Code**

	BE_ClipboardSetFile ( table::containerField ; "com.adobe.pdf" )

Sets the clipboard with a PDF from a field.
