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

This is a complete list of all the xslt functions you can use in BE, as dug out of the code by Beverly Voth :

	<?xml version="1.0"?>
	
	=== 24 Standard elements:
	xsl:apply-templates available
	xsl:apply-imports available
	xsl:call-template available
	xsl:element available
	xsl:attribute available
	xsl:text available
	xsl:processing-instruction available
	xsl:comment available
	xsl:copy available
	xsl:value-of available
	xsl:number available
	xsl:for-each available
	xsl:if available
	xsl:choose available
	xsl:sort available
	xsl:copy-of available
	xsl:message available
	xsl:variable available
	xsl:param available
	xsl:with-param available
	xsl:decimal-format available
	xsl:when available
	xsl:otherwise available
	xsl:fallback available
	
 	=== 5 Extension elements:
	xsl:element available
	saxon:output available
	xalanredirect:write available
	xt:document available
	libxslt:debug available
 	
	 === 6 Extension functions:
	libxslt:node-set() available
	saxon:node-set() available
	xt:node-set() available
	saxon:evaluate() available
	saxon:expression() available
	saxon:eval() available