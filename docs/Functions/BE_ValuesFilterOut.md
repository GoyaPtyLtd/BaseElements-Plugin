## BE_ValuesFilterOut

	BE_ValuesFilterOut ( textToFilter ; filterValues ; { caseSensitiveBoolean } )

**Description**  

Does the opposite of the *FilterValues* function : it takes the *textToFilter* parameter, and removes anything in the *filterValues* list and returns the remaining values.

**Parameters**

* *textToFilter* : The list of values to start with.
* *filterValues* : The values to remove from textToFilter.
* *caseSensitiveBoolean* ( optional, default:True ) : Whether to treat "ABC" the same as "abc" or not.

**Keywords**  

Values Filter Out

**Version History**

* 2.1.0 : First Release
* 3.1.0 : Added the optional caseSensitive parameter
* 4.0.2 : Renamed from BE_Values_FilterOut

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

	BE_Values_FilterOut ( "a¶b¶c¶d" ; "b¶d" ) = "a¶c"
