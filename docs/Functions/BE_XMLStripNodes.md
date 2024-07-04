## BE_XMLStripNodes

    BE_XMLStripNodes ( inputFilePath ; outputFilePath ; nodeNames )

**Description**  

This function removes the supplied list *nodeNames* of XML nodes from the source document at *inputFilePath* and writes the result to *outputFilePath*.

**Parameters**

* *inputFilePath* : the plugin path to the XML file to read from.
* *outputFilePath* : the plugin path to write the output to.
* *nodeNames* : a space separated list of xml node names to remove.

**Keywords**  

XML Strip Nodes Path

**Version History**

* 4.2.0 : First Release

**Notes**

Previously an internal hidden function, was made public in 4.2.0.

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

To remove the *HexData* and *PlatformData* nodes from an XML DDR file, you can use :

	BE_XMLStripNodes ( $inputFilePath ; $outputFilePath ; "HexData PlatformData" )
