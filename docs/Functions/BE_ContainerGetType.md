## BE_ContainerGetType

    BE_ContainerGetType ( container ; type )

**Description**  

Gets the content of the field container in the format of type. Types are only what is available from doing  *BE_ContainerListTypes* first. If the type does not exist, it cannot be retrieved.

**Parameters**

* *container* : the container field or binary variable to use as the source.
* *type* : the type of binary data to get.

**Keywords**  

keyword keyword

**Version History**

* 4.1.0 : First Release

**Notes**

Container field types are available by doing BE_ContainerListTypes and might change depending on FileMaker version, storage method, or other factors beyond the plugins control.  

Mostly if you've stored a file, then the only useful format will be "FILE".  When images are stored FileMaker often converts the image to other formats such as "JPEG".

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

	BE_ContainerGetType ( table::field ; "JPEG" )

If you've stored a PDF in the field, then this will often retrive the first page of the PDF as a JPEG.