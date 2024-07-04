## BE_VectorEuclideanDistance

    BE_VectorEuclideanDistance ( a ; b )

**Description**  

The euclidean distance between two vector points.

**Parameters**

* *a* : vector a as a list.
* *b* : vector b as a list.

**Keywords**  

Vector DotProductkeyword

**Version History**

* 3.2.0 : First Release
* 4.0.2 : Renamed from BE_Vector_EuclideanDistance

**Notes**

For more information about this see :

[https://en.wikipedia.org/wiki/Euclidean_distance](https://en.wikipedia.org/wiki/Euclidean_distance) 

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

	BE_VectorEuclideanDistance ( "1¶3¶-5" ; "4¶-2¶-1" ) = 7.07107
