## BE_FileImport

    BE_FileImport ( filePath ; { compressBoolean } )

**Description**  

Imports the contents of the file at *filePath* into a container field or variable, and optionally compresses the field using the built in gzip compression. This provides a similar functionality to the Insert File script step, but the plugin function is also available via FileMaker Server whereas the step doesn't work on server.

**Parameters**

* *filePath* : The path to locate the file.
* *compressBoolean* ( optional, default:True ) : True for compression or False for uncompressed.

**Keywords**  

File Import Compression

**Version History**

* 3.0.0 : First Release
* 4.0.2 : Renamed from BE_ImportFile

**Notes**

Also note that you can convert container types within a container via the *BE_ConvertContainer* function, so you can convert the resulting file to an image if required.

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

	Set Field [ Imports::Image; BE_FileImport ( $file ) ]
