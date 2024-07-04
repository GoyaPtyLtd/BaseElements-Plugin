## BE_XSLTApply

	BE_XSLT_Apply ( xmlFilePath ; xsltText ; outputFilePath {; scriptName ; databaseName ; [ xsltText ; outputFilePath ] ; ... } )

**Description**  

Applies the XSLT given by the *xsltText* parameter, to the XML file at *xmlFilePath*, and writes the output to *outputFilePath*.

**Parameters**

* *xmlFilePath* : the plugin path to the xml file to process.
* *xsltText* : the text content of the xslt to process with.
* *outputFilePath* : the plugin path to write the output result to.
* *scriptName* : a script to call on completion of the transform.
* *databaseName* : the database where scriptName resides.

To launch the XSLT process in its own thread add the *scriptName* and optional *databaseName* parameter.  When the *scriptName* parameter is used, then the XSLT process is launched in a background thread and control returns to FileMaker immediately.

This function also allows you to launch multiple threads of the XSLT processing to happen simultaneously by appending groups of two extra parameters after the databaseName parameter, with each additional *xsltText* and *outputFilePath* in pairs.

**Keywords**  

XML XSLT Memory

**Version History**

* 1.0.0 : First Release
* 5.0.0 : Added scriptName, and optional extra parameters.

**Notes**

When the *scriptName* option is used, the script parameter sent to *scriptName* is the response from the XSLT function if present, or the *outputFilePath* otherwise.

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
