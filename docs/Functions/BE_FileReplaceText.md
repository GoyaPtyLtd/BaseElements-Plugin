## BE_FileReplaceText

    BE_FileReplaceText ( pathOrContainer ; expression ; replaceString { ; options } )

**Description**  

Much like the native Substitute function, but for a text file on disk or stored inside a container field.

**Parameters**

* *pathOrContainer* : either a full path to the file on disk, or a container field.
* *expression* : The text string to look for in the file.
* *replaceString* : The text string to replace it with in the file.
* *options* ( optional, defaut:gi ) : A string of characters from the options below , eg "igm".

If the *pathOrContainer* is a container field which contains only text, then it will be read as a path, and that path will be used to locate the file.

**Keywords**  

File Replace Text Path

**Version History**

* 4.2.0 : First Release

**Notes**

Options :
* i - case insensitive
* m - multiline
* s - dot matches all characters, including newline<
* x - ignore whitespace
* g - replace all

For options: Default is "gi" which matches the native FileMaker substitute. These options work exactly the same as the RegularExpression function.

PLUGIN PATHS ARE NOT FILEMAKER PATHS. The plugin uses the same path structure as your operating system, and you cannot pass to the plugin paths that start with file:/ or filewin:/ etc.  Read the FAQ for more info about paths.

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
