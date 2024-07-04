## BE_JSON_ArraySize

    BE_JSON_ArraySize ( json { ; path } )

**Description**  

Will return the number of values in a JSON array.

**Parameters**

* *json* : the array to count.
* *name* ( optional ) : the path within the JSON to locate the array to count.

**Keywords**  

JSON Array Size

**Version History**

* 2.1.0 : First Release
* 4.1.0 : Added the optional path parameter

**Notes**

If *path* is not included, it counts the array at the root of the JSON, and will return 1 for a non array type.

If *path* is included, the format is as per the *BE_JSONPath* function, which is NOT the same as the internal FileMaker JSON functions introduced in version 16.

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
