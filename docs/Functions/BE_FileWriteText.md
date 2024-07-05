## BE_FileWriteText

    BE_FileWriteText ( pathOrContainer ; text ; { appendBoolean } )

**Description**  

Writes the contents specified in *text* to the file at the *pathOrContainer* or to a container field via Set Field. 

**Parameters**

* *pathOrContainer* : either text which contains a path to a file, or binary data from a container field, or file set into a variable.
* *text* : the text to write to the file.
* *appendBoolean* ( optional, default:False ) : True will append to the file, False or no parameter will either write over an existing file or set the contents of a new file.

Using the optional *appendBoolean* parameter you can choose to either write a new file ( or overwrite an existing one ), or to append the text to the end of the file.

**Keywords**  

File Text Write Path Container

**Version History**

* 1.0.0 : First Release
* 1.1.0 : Adds appendBoolean option
* 3.3.0 : Recursively create any directories needed
* 4.0.2 : Renamed from BE_WriteTextToFile
* 4.0.2 : Allows writing to container fields as well as paths

**Notes**

Defaults to UTF-8 ( no BOM ) which can be changed using the BE_SetTextEncoding function.

For writing files to containers, this function works a little differently. First you need to use this as a Set Field step, and the resulting output will then be set into the field that is the destination of the Set Field step.

If you're appending to a file, and setting it into a field, the first parameter pathOrContainer can be either a field with an existing file, or a path to an existing file. The result will then be appended to the original. Set the appendBoolean parameter to True.

If you're setting a field and not appending, the first parameter should contain just the name of the file you want as text.

Obviously this first parameter can be many things, a path, a container field containing a file, or a text string ( from a field, or variable etc ) containing a file name. 

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

	BE_FileWriteText ( $path ; "sometexthere" )
	
creates a new file at $path with the content "sometexthere"

	BE_FileWriteText ( $path ; "sometexthere" ; True )
	
appends to an existing file at $path and adds "sometexthere" to the end of the document

	SetField [ table::myField ; BE_FileWriteText ( $path ; "sometexthere" ; True )
	
sets myField with a file, that is the content of the existing file at $path with "sometexthere" appended. Filename will be as per the existing file at $path.

	SetField [ table::myField ; BE_FileWriteText ( "myfile.txt" ; "sometexthere" )
	
Creates a new file called "myfile.txt" and sets the content to "sometexthere" and puts it into the myField field.