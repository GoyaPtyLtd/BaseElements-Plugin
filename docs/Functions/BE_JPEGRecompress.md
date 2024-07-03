## BE_JPEGRecompress

    BE_JPEGRecompress ( jpeg ; { compressionLevel ; scale } )

**Description**  

Re-compresses the jpeg image file found at the container field *jpeg*, using a *scale* factor and *compressionLevel*.

**Parameters**

* *jpeg* : A JPEG file stored in a container.
* *compressionLevel* ( optional, default:75 ) : A value between 1 and 100 (inclusive). The default value, for no compelling reason, is 75.
* *scale* ( optional, default:0.125 ) : A number for the scale of the resulting image.  The default value is 0.125.

**Keywords**  

JPEG Compress Recompress

**Version History**

* 3.1.0 : First Release
* 3.2.0 : Changed the height and width to a scaling factor to better match the actual workings of the library used.
* 3.1.0 : Renamed from BE_JPEG_Recompress.

**Notes**

Use this function with a *Set Field* script step to convert one container to another.  It can be used to set the same field as the one referenced in the function parameter.

Values for the "scale" parameter are :

2
1.875
1.75
1.625
1.5
1.375
1.25
1.125
1
0.875
0.75
0.625
0.5
0.375
0.25
0.125

Any other values are rounded down to the nearest value, except values below 0.125 where it will use 0.125.

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

