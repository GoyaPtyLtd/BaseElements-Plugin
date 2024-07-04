## BE_XPathAll

    BE_XPathAll ( xmlText ; xpathText ; { namespaceListText } )

**Description**  

This finds all instances of a node at the path xpath within the text xml.  Similar to running *BE_XPath* in a loop. 

**Parameters**

* *xmlText* : the XML text to use.
* *xpathText* : the path to the node you require.
* *namespaceListText* ( optional ) : Namespace list as "prefix1=href1 prefix2=href2...".

**Keywords**  

XML XPath

**Version History**

* 1.0.0 : First Release
* 1.2.0 : 

**Notes**

This function is based on the libxml library which only supports XPath 1.0 [http://xmlsoft.org]

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

	BE_XPath ( $XML ; "/abc:checkUserResponse/abc:result/abc:accountDetails/@type" ; "abc=http://xml.m4u.com.au/2009" )