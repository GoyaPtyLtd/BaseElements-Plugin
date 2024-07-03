## BE_ContainerCompress

    BE_ContainerCompress ( data ; { filename } )

**Description**  

Converts stored container field data between the internal un-compressed and compressed formats - does not alter the content of the container as far as the end user sees.  This only adjusts how much storage it takes up on disk.

The internal compression format is gzip, there are no other compress options.

**Parameters**

* *data* : the uncompressed container field.
* *filename* : the filename to use.  If left out, the plugin will attempt to return text, which probably won't work for most uses of gzip.

**Keywords**  

Container Compress Gzip

**Version History**

* 3.1.0 : First Release
* 3.2.0 : Changed name from BE_Gzip to BE_ContainerCompress

**Notes**


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

