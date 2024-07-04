## BE_XMLStripInvalidCharacters

    BE_XMLStripInvalidCharacters ( path ; { resultFilePath } )

**Description**  

This function removes characters considered invalid in XML from the file at *path*, and optionally writes the fixed file to the *resultFilePath*.

**Parameters**

* *path* : The path to the text XML file to read.
* *resultFilePath* ( optional, default:path ) : If supplied, this will write the output to a new file. If not supplied then the original file will be overwritten.

**Keywords**  

XML Strip Invalid Characters

**Version History**

* 4.2.0 : First Release

**Notes**

Used originally internal as a BaseElements developer tool function, likely not useful for anyone else.

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
