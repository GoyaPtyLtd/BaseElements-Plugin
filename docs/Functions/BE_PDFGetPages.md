## BE_PDFGetPages

    BE_PDFGetPages ( pdfPathOrContainer ; newPDFPath ; fromPageNum ; { toPageNum } )

**Description**  

Copies a number of pages from the PDF at *pdfPathOrContainer* and creates a new document written to disk at *newPDFPath*, using only pages starting with page number *fromPageNum* and ending with page number *toPageNum* or the end of the document.

**Parameters**

* *pdfPathOrContainer* : The original file to GET the pages from, either a container field or a path.
* *newPDFPath* : The path to write the result file to.
* *fromPageNum* : The starting page number to begin from.
* *toPageNum* ( optional ) : The page number to end on, or if left out will go to the end of the document.

**Keywords**  

PDF Page Get Path

**Version History**

* 4.0.0 : First Release
* 4.0.2 : Renamed from BE_PDF_GetPages
* 4.2.0 : Added a "garbage collection" option so that the resulting file size is reduced as well

**Notes**

If the plugin can't read the file, but it is a PDF then you may get an invalid result from this function.  See the notes at *BE_PDFAppend* for more information.

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
