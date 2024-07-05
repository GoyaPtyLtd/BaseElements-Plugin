## BE_ClipboardGetText

    BE_ClipboardGetText ( format )

**Description**  

Tries to convert the item on the current clipboard, designated by the *format* passed to the function.

The *format* used must be one of the values from the BE_ClipboardFormats function.  

The function will only deal with text clipboards, so other clipboard types are ignored. To get binary types use BE_ClipboardGetFile.

**Parameters**

* *format* : This has to be one of the formats on the current clipboard.

**Keywords**  

Clipboard Text Get

**Version History**

* 1.0.0 : First Release.
* 4.0.0 : Return error 3, command not available on Linux.
* 4.0.0 : Add utf16 support on Mac.
* 4.0.2 : Renamed from BE_ClipboardText.

**Notes**

The plugin will try to store the result, but cannot guarantee exact text conversion from the clipboard. The clipboard can be of many different types, and even if it's text it can be stored with different encodings. It will depend on where you're storing the result ( field, variable, on disk, back to plugin etc ) and how ( direct in the plugin, via step, via field etc ), as to whether it's an exact match for the clipboard.

The *BE_SetTextEncoding* function does have an effect on how this result is used within the plugin.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | No |  
| iOS | No  |  
| Linux | No  |  

**Example Code**

	BE_ClipboardGetText ( "dyn.ah62d4rv4gk8zuxnxnq" )

Turns a copied FileMaker Script Step into the text XML representation.
