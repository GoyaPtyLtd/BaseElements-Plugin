## BE_ClipboardGetFile

    BE_ClipboardGetFile ( format ; { fileName } )

**Description**  

Retrieves a binary item from the current clipboard, designated by the *format* passed to the function.

The *format* used must be one of the values from the *BE_ClipboardFormats* function.  

Use this function as the calculation for a *Set Field* script step when setting a container field, or when setting a variable to a container type.

**Parameters**

* *format* : The type of the format to get from the list of formats on the current clipboard.
* *fileName* ( optional, default:existing file name if included ) : Optional parameter to set a resulting filename, but can be left out if the clipboard already has a filename.

**Keywords**  

Clipboard Data Get

**Version History**

* 4.0.3 : First Release

**Notes**

If the format is a binary file type, and has a filename, then that parameter is not required - not all types include a filename, and it's not always possible to get the filename from all types. Otherwise a filename is recommended so that the result is useful inside FileMaker.

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

	BE_ClipboardGetFile ( "com.adobe.pdf" ; "Object.pdf" )
	
Used in a Set Field step to get the pdf of a copied layout object. In this case the clipboard would also contain the text XML description under a different "format", plus possibly a plain text "format" with some of the layout content.

The result is not an image at this point, it's still a file.  You can convert to an image with :

	BE_ConvertContainer ( BE_ClipboardGetFile ( "com.adobe.pdf" ; "Object.pdf" ) ; "PDF " )

Converts the above file container into an image container ( note the space after PDF, all types are 4 characters ).
