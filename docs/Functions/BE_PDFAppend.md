## BE_PDFAppend

    BE_PDFAppend ( pdfPathOrContainer ; appendPathOrContainer ; { destinationPath } )

**Description**  

Joins two pdf files together, either by appending one to the first, or by creating a third PDF that is the end result.

**Parameters**

* *pdfPathOrContainer* : The first PDF file to be appended TO. Can be a container field or a path to a file.
* *appendPathOrContainer* : The second file, to be added to the first file. Can be a container field or a path to a file.
* *destinationPath* ( optional, default:container ) : the path to the result file.  If left out, then use this in a Set Field step that points to a container field.

**Keywords**  

PDF Path Append

**Version History**

* 4.0.0 : First Release
* 4.0.0 : Renamed from BE_PDF_Append

**Notes**

Some PDFs although they render properly in a PDF Viewer are incompatible with the PDF standard or at least with the version of the standard that our library supports. When this is the case you can get an error when trying to append the PDF.

To diagnose this before appending if you do *BE_PDFPageCount ( table::field )* on the PDF and you get 0 back from the plugin, then our library doesn't think there's any content, so it can't be appended.

You should be adding this check in to your script prior to using *BE_PDFAppend*, to confirm that both files contain data that can be read.

In terms of fixing the PDF, there's no way to do that inside the plugin at the moment.

You may be able to do this via an external library such Ghostscript. This option requires Ghostscript to be installed on your server or client, and then you run the command using *BE_ExecuteSystemCommand*.

There are other APIs that offer to do PDF repair such as : 

[https://www.convertapi.com/pdf-to-repair#snippet=curl](https://www.convertapi.com/pdf-to-repair#snippet=curl) 

Which can be called with the BE HTTP functions, or the native *Insert From URL* script step. However the extent to which they can repair a broken PDF vs a non standard PDF is not known. There may also be other similar services online, or local applications as alternatives.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | Yes  |  
| iOS | No  |  
| Linux | Yes  |  

**Example Code**

Set a container field using two other fields.

	Set Field [ table::containerField ; BE_PDFAppend ( table::pdfField1 ; table::pdfField2 ) ]

Set a container field using two paths.

	Set Field [ table::containerField ; BE_PDFAppend ( "/pluginPath/to/file1.pdf" ; "/pluginPath/to/file2.pdf" ) ]

Create an external PDF file by combining a container field and a different external file.

	Set Variable [ $result ; BE_PDFAppend ( table::pdfField1 ; "/pluginPath/to/file2.pdf" "/pluginPath/to/outputFile.pdf" ) ]
