## BE_FileReadText

    BE_FileReadText ( pathOrContainer { ; start ; to ; eolChar } )

**Description**  

Reads the contents of the file at *pathOrContainer* as text and returns the content, optionall beginning at point *start*, ending at *to* and treating *eolChar* as line characters.

**Parameters**

* *pathOrContainer* : description.
* *start* ( optional ) : description.
* *to* ( optional ) : description.
* *eolChar* ( optional ) : description.

Use *start*, *to* and *eolChar* to extract a portion of the file - the first character is at position 0 which is different from the native Position function.

Pass *start* as empty or greater than *to* to read from the end of the file in reverse back to *to* - useful for getting the tail of log files for example.

Pass an empty *eolChar* to use start and to as pure character counts. 

If you include a value for *eolChar* it will treat *eolChar* as a line break character, and will return lines of text instead of characters of the text.  So *start* as the first line to start at, and *to* the last line.

**Keywords**  

File Text Read Path

**Version History**

* 1.0.0 : First Release
* 4.0.0 : Allow reading text from a container file instead of a path
* 4.0.2 : Renamed from BE_ReadTextFromFile
* 4.2.0 : added start, to and eolChar parameters
* 5.0.0 : Fixed a bug in the way that the function works on container fields to match the external files behaviour
* 5.0.0 : Fixed a bug in the eolChar to allow multiple character eol values

**Notes**

PLUGIN PATHS ARE NOT FILEMAKER PATHS. The plugin uses the same path structure as your operating system, and you cannot pass to the plugin paths that start with file:/ or filewin:/ etc.  Read the FAQ for more info about paths.

Note that *eolChar* doesn't have to be a normal end of line character such as a line feed or carriage return. You can use any character you want. This also means that text brought into FileMaker may not have multiple lines in the sense of what a FileMaker value list has. Or another way : the plugin doesn't modify the text to replace your *eolChar* with an end of line character.

So you could use "," as the *eolChar* to get certain columns out of a csv file.

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

	BE_FileReadText ( $path )

reads the entire file

	BE_FileReadText ( $path ; 1 ; 100 )

reads the first 100 characters

	BE_FileReadText ( $path ; 1 ; 10 ; char ( 10 ) )

reads the first 10 lines of the file

	BE_FileReadText ( $path ; 10 ;  ; char ( 10 ) )

reads from line 10 to the end of the file

	BE_FileReadText ( $path ;  ; 10 ; char ( 10 ) )

reads the last 10 lines of the file.