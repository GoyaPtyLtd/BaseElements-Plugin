## BE_PDFPageCount

    BE_PDFPageCount ( pdfPathOrContainer )

**Description**  

Counts the number of pages in a PDF file.

**Parameters**

* *pdfPathOrContainer* : The original file to count the pages of, either a container field or a path.

**Keywords**  

PDF Page Count Path

**Version History**

* 4.0.0 : First Release
* 4.0.0 : Renamed from BE_PDF_PageCount

**Notes**

If the plugin can't read the file, but it is a PDF then you may get a 0 from this function.  See the notes at *BE_PDFAppend* for more information.

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
