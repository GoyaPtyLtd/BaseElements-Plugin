## BE_RegularExpression

	BE_RegularExpression ( text ; expression ; { options ; replaceString } )

**Description**  

A perl compatible regular expression testing function.

**Parameters**

* *text* : The text to be examined.
* *expression* : A "perl compatible" expression - see examples.
* *options* : A string of characters from the options below , eg "igm".
* *replaceString* : If not empty, then a replace is performed rather than a find.

**Keywords**  

RegularExpression Regular Expression Regex

**Version History**

* 3.2.0 : First Release
* 4.0.05: Added an extra option "v" to allow you to treat the input as a value list and perform the expression over all values in the list.

**Notes**

Options :
* i - case insensitive
* m - multiline
* s - dot matches all characters, including newline<
* x - ignore whitespace
* g - replace all
* v - treat the input as a value list and iterate over each value.

More examples can be found here : [http://perldoc.perl.org/perlre.html#Regular-Expressions](http://perldoc.perl.org/perlre.html#Regular-Expressions)

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

	BE_RegularExpression ( "abc 123 def" ; "[0-9]+" ) = 123
	BE_RegularExpression ( $list; "(?:\\S(?![ ]))+$"; "igm" )
	
To get a list of all the file paths that end in ".fmp12" : 

	BE_ValuesUnique ( BE_RegularExpression ( BE_FileListFolder ( $path ) ; "^.*\\.fmp12$" ; "v" ) )
	
The above example uses *BE_ValuesUnique* because the function leaves empty lines where the criteria doesn't match, so you'd have as many "values" as there were originally files, but anything not matching the criteria would be blank text in the list.


