## BE_ArrayDelete

    BE_ArrayDelete ( array )

**NOTE This function as of current release, has a bug.  If you create more than one array, and delete an earlier one, it shuffles the index.  So create array 1 and 2 and 3 and delete 2, then array number 3 moves into the 2 spot.  This is a bug and will be fixed in a future release.  Our suggestion for now is not to delete old arrays after adding new ones.**

**Description**  

Removes *array* from memory.

**Parameters**

* *array* : the number of the array to be deleted.

**Keywords**  

Array Delete

**Version History**

* 4.0.0 : First Release
* 4.0.2 : Renamed from 'BE_Array_Delete'

**Notes**

Be careful, there is no undo

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
