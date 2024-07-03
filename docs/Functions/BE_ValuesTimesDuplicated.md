## BE_ValuesTimesDuplicated

	BE_ValuesTimesDuplicated ( listOfValues ; numberOfTimes )

**Description**  

Select only those values from *listOfValues*, where the value has been repeated *numberOfTimes*.  In other words, filter the list to only those values that are repeated numberOfTimes.

**Parameters**

* *listOfValues* : The list of values to look through.
* *numberOfTimes* : The count to check for in the list.

**Keywords**  

Values Duplicates Times

**Version History**

* 3.2.0 : First Release
* 4.0.2 : Renamed from BE_Values_TimesDuplicated

**Notes**

Notes

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

	BE_ValuesTimesDuplicated ( "a¶c¶c¶d¶d" ; 2 ) = "c¶d"
	BE_ValuesTimesDuplicated ( "a¶c¶c¶d¶d" ; 3 ) = ""
