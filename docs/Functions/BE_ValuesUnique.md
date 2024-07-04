## BE_ValuesUnique

	BE_ValuesUnique ( listOfValues ; { caseSensitiveBoolean } )

**Description**  

Removes duplicate values from listOfValues.

**Parameters**

* *listOfValues* : The list of values to start with.
* *caseSensitiveBoolean* ( optional, default:True ) : Whether to treat "ABC" the same as "abc" or not.

**Keywords**  

Values Unique List

**Version History**

* 2.1.0 : First Release
* 3.1.0 : Added caseSensitiveBoolean option
* 4.0.2 : Renamed from BE_Values_Unique

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

	BE_ValuesUnique ( "a¶a¶b¶b" ) = "a¶b"