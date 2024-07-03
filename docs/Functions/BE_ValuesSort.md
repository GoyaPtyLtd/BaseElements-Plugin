## BE_ValuesSort

	BE_ValuesSort ( listOfValues ; { ascendingBoolean ; type } )

**Description**  

Sorts *listOfValues* according to the ascending order *ascendingBoolean* and data *type*.

**Parameters**

* *listOfValues* : The list of values to start with.
* *ascendingBoolean* ( optional, default:1 ) : 1 for ascending, 0 for descending.
* *type* ( optional, default:0 ) : A number corresponding to the value for different types.  At the moment, only two values are defined : 0 for Text, 1 for Numeric.

**Keywords**  

Values Sort

**Version History**

* 2.1.0 : First Release
* 3.3.0 : Added the ascending and type parameters
* 4.0.2 : Renamed from BE_Values_Sort

**Notes**

In some situations this sort order won't be the same as the order that FileMaker would apply in a sorted field.  Different encoding and "type" arrangements treat values in different ways, so there's no expectation that the BE sort will be the same as the FM one - the technical details into why this is gets really complex as you get into text encoding and storage and values and ... If you're after a good explanation of why this is no longer a simple thing, this is a good reference : [https://tonsky.me/blog/unicode/](https://tonsky.me/blog/unicode/)

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

