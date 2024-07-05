## BE_UnGzip

    BE_UnGzip ( gzip_data ; { filename } )

**Description**  

Uncompresses the file found in *gzip_data* and optionally applies the *filename*, otherwise uses the detail found in the container field if it exists.

**Parameters**

* *gzip_data* : a container field with an gzipped file in it.
* *filename* : optional filename to use, appending the .gzip extension.

**Keywords**  

Gzip zip Ungzip unzip

**Version History**

* 3.3.0 : First Release

**Notes**

If you're unzipping a file, then this function should be used with a *Set Field* script step and the results stored as a container field.  If you're expecting a text result then this could be used anywhere text is expected.
 
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
