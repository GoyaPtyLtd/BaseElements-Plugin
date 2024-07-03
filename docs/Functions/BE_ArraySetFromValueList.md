## BE_ArraySetFromValueList

    BE_ArraySetFromValueList ( valueList ; { retainEmptyValues } )

**Description**  

Stores the valueList as an array within the plugin memory space and returns an index number that you can use to reference the array via memory.

**Parameters**

* *valueList* : The standard FileMaker return separated list of values to store.
* *retainEmptyValues* ( optional, default:False ) : Whether or not to include empty values as array elements.

**Keywords**  

Array Set ValueList

**Version History**

* 3.3.0 : First Release
* 4.0.0 : added the *retainEmptyValues* parameter

**Notes**

BE plugin value lists have by default a difference when compared to FileMaker value lists.  The default option for *retainEmptyValues* ignores empty values, so "a¶¶b¶" becomes an array of [a,b] - in other words an array of two values.  Be careful if you're assuming that empty values are retained, or set the *retainEmptyValues* parameter to True if you want to preserve FileMaker native list expectations.

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

