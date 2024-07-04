## BE_XMLParse

    BE_XMLParse ( pathOrXMLText )

**Description**  

Parses an XML file at path to determine if it's properly formed. Does not check XSD details, just whether every tag is properly opened and closed.

The result will be empty when successful with a *BE_GetLastError* returning 0.

**Parameters**

* *pathOrXMLText* : the xml text or a plugin path to locate the file to parse.

**Keywords**  

XML Parse Path

**Version History**

* 2.2.0 : First Release
* 4.0.2 : Renamed from BE_XML_Parse

**Notes**

This function will look at the content of the text in the pathOrXMLText parameter, and if the first character is < then it assumes it is XML and treats it as such.  Any other first character means it assumes it's a path and will try to use it as a path on disk.

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
