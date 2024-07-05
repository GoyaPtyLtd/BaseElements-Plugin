## BE_XPath

    BE_XPath ( xmlText ; xpathText ; { namespaceListText ; asTextBoolean } )

**Description**  

This finds the first instance of a node at the path *xpathText* within the text *xmlText*. 

**Parameters**

* *xmlText* : the XML text to use.
* *xpathText* : the path to the node you require.
* *namespaceListText* ( optional ) : Namespace list as "prefix1=href1 prefix2=href2...".
* *asTextBoolean* ( optional, default:False ) : Whether to return the XML nodeset as text instead of the standard XPath result.

The *asTextBoolean* parameter when set to True allows you to get raw XML from the source - normally where the XPath function would return a node, with it's opening and closing tag, this will return the value inside that tag instead.

**Keywords**  

XML XPath

**Version History**

* 1.0.0 : First Release
* 1.2.0 : 
* 2.2.0 : Added the optional asText parameter

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