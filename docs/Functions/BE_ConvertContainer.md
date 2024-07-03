## BE_ConvertContainer

    BE_ConvertContainer ( field ; { type } )

**Description**  

This is to convert container field contents between the internal FileMaker "image" type and the internal "file" type. So you've used the Insert File script step to store an image file in a container, but then want that container to display the image content instead of a file icon.

**Parameters**

* *field* : a container field with a file or image in it.
* *type* ( optional, default:empty ) : either blank for a type of "file" or an option from the list below.

**Keywords**  

Container Convert

**Version History**

* 3.1.0 : First Release
* 4.2.0 : Removed the width and height parameters, the plugin now performs this natively.

**Notes**

This is not an image format conversion function so you cannot use to convert a png to jpeg etc.  Nor does it resize images. 

It only converts between the two container types, of file and image.  When going from file to image, FileMaker needs to be told what sort of image it is, and that's what you need to pass in the parameters.

If you need to resize a JPEG image file, you can use the *BE_JPEGRecompress* function.

Use this function with a Set Field script step to convert from a container field stored as a file into a container image or vice versa. It can be used to set into the same field as the one referenced in the function parameter.

1. If only the field is supplied then it is converted to “FILE”
2. If “ZLIB” is supplied the file is compressed
3. Other known formats are ( all four character codes - note the space after PDF ):

"snd " *
"JPEG”"
"GIFf" *
"EPS " *
"META" *
"PNGf"
"BMPf" *
"PDF "

 * = not tested.

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

