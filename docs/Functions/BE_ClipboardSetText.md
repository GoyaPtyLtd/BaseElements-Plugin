## BE_ClipboardSetText

    BE_ClipboardSetText ( text ; format )

**Description**  

Sets the clipboard to the *text* and applies a clipboard **format**. Clipboard formats are set by the application that does the copy and vary widely based on the content types.

Use the **BE_ClipboardFormats** function to get examples of different types. The format you choose should depend on what you expect the user to do with the clipboard content.

This function only works with text, to set non-text data use **BE_ClipboardSetFile**.

**Parameters**

* *text* : The text to set the clipboard to.
* *format* : The format code for the content.

**Keywords**  

Clipboard Text Set

**Version History**

* 1.0.0 : First Release
* 4.0.0 : Return error 3, command not available on Linux.
* 4.0.2 : Renamed from BE_SetClipboardText

**Notes**

When setting a type it's best to copy a sample and then use *BE_ClipboardFormats* to see what types are used.

Also note that many of the windows text formats are special "null terminated strings". Meaning that they expect the text to have an extra character on the end, and if you don't include them, then they will chop the last character off the end of your text. The full windows docs can be found here : [https://docs.microsoft.com/en-us/windows/win32/dataxchg/standard-clipboard-formats](https://docs.microsoft.com/en-us/windows/win32/dataxchg/standard-clipboard-formats). 

See the examples for a method to add the null terminated string to your text.

You can only set a single format when setting the clipboard from BE.

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

	BE_ClipboardSetText ( $ScriptStepXML ; "dyn.ah62d4rv4gk8zuxnxnq" )

Turns XML text into a FileMaker Script Step ready to be pasted back into FileMaker. The example above uses a special code that FileMaker uses "dyn.ah62d4rv4gk8zuxnxnq" to have a unique type of clipboard text, that only FileMaker knows about. So even though it's text, no other text app recognises this and so won't paste it.

	BE_ClipboardSetText ( TABLE::field & Char ( 0 ) ; "CF_TEXT" )

An example of a NULL terminated string that you set onto the clipboard for the specific windows text type.