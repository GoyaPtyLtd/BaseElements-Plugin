## BE_TextExtractWords

    BE_TextExtractWords ( text ; { wordPrefix } )

**Description**  

This returns a list of all the "words" in the *text* that start with the *wordPrefix* character.

**Parameters**

* *text* :  the text to search on.
* *wordPrefix* ( optional, default:$ or @ ) : a single chararacter to use as the first character of the words to find.

**Keywords**  

keyword keyword

**Version History**

* 1.0.0 : First Release

**Notes**

Originally an internal only function, this was not exposed as an option in the functions list but as of 4.2, we've made this one visible in case it's useful to others.

Our use case for this was to use it inside the BaseElements developer tool engine, to retrieve all the variables from a block of calculation text, but getting anything that starts with $.  We expanded it to @ for other  similar functionality.

Words are delimited ( ended ) by one of the following characters : 

	; +-=*/&^<>\t\r[]()\u2260\u2264\u2265,

Note that this list of characters includes a space.  The ones starting with \u are single unicode characters.

Also note that this function leaves out any text within FileMaker comments, so from // to the endOfLine and from /* to */  So it won't find any words inside those characters.

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

	BE_TextExtractWords ( "apple bear $ball" ) = "$ball"