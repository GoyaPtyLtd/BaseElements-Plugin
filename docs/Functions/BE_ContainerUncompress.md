## BE_ContainerUncompress

    BE_ContainerUncompress ( gzip_data ; { filename } )

**Description**  

Converts stored container field *gzip_data* between the internal compressed and un-compressed formats.

**Parameters**

* *gzip_data* : the compressed container field.
* *filename* : the filename to use.

**Keywords**  

Container Compress Uncompress

**Version History**

* 3.1.0 : First Release
* 3.2.0 : Changed name to BE_ContainerUncompress from BE_UnGzip.

**Notes**

The internal compression format is gzip.

When inserting files into containers in FileMaker, you have the option to "compress" the file.  This function can change an compressed file into a uncompressed file.

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

