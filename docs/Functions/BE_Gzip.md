## BE_Gzip

    BE_Gzip ( data ; { filename } )

**Description**  

Compresses the file/text found in *data* and optionally applies the filename, otherwise uses the container data appended with the .gzip extension.

**Parameters**

* *data* : a container field with an unzipped file in it, or text data.
* *filename* : optional filename to use, appending the .gzip extension.

**Keywords**  

Gzip zip

**Version History**

* 3.3.0 : First Release

**Notes**

This function should be used with a *Set Field* script step and the results stored as a container field.

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
