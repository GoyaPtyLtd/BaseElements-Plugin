## BE_ExportFieldContents

    BE_ExportFieldContents ( field ; { outputPath }  )

**Description**  

Exports the contents of the container *field* to disk at the *outputPath* specified. Container fields should be inserted via "Insert File". This provides a similar functionality to the *Export Field Contents* script step, but this funciton is also available on FileMaker Server, whereas the script step is not.

**Parameters**

* *field* : The field to export.
* *outputPath* ( optional, default:temp ): The path to write to.

**Keywords**  

keyword keyword

**Version History**

* 3.0.0 : First Release
* 3.3.0 : Recursively create any directories needed
* 4.0.0 : Allowed for images with resource forks
* 4.0.0 : Changed the outputPath parameter to be optional.

**Notes**

If not supplied it will write the file to the temp folder and return the path to the file.

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

