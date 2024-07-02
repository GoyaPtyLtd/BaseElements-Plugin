## Feature Requests and Ideas

Mostly just preserved from the old docs site, will be migrated to Issues eventually.

Completed items in 4.0 :
iOS support.
BE_FileSize that also works on folders.
Include v16 descriptions and allow translated descriptions via git.
Add a File_ContainerToTemp function to save a file to disk in the temp folder and return a path to the file, to allow any function that uses paths to also use files.
BE_ArrayFind ( array01 ; ValueToFind ) returns index number.
BE_ArrayChangeValue ( array02 ; FoundIndexNumberArray01 ; NewValue )
Add priority or other headers to emails.
Adjust plugin parameters to allow path or container on most functions.
Renaming of functions and parameters to clear up and group things better.
Thread safety for the XML / XSLT functions.
XML Schema Validation
Work on thread safety for XSLT.  Plus evaluating which other functions are thread safe, or not.
PDF - Get Page Count

Changes planned for a 4.x minor update :
Duplicate the HTTP_PUT_File to HTTP_POST_File to allow octect-stream format POST.
Some conversion of functions to script steps where appropriate
Integration of the ImageMagick library.
Video - Thumbnails and metadata : https://baseelements.zendesk.com/agent/tickets/1839
JSON to Array functionality.
More thread safety changes to other functions where available.
Add an optional overwrite parameter to the Copy and Move functions.

Changes planned for the next major release ( 5.0 ) :
Changing the MIME library we use to allow multipart/related attachments and therefore inline images in HTML email.  Or options to allow the inserting of images that can then be referenced in manually generated HTML later.
PDF Splitting - taking pages out of a PDF file and optimisation.
SMTP connect and disconnect step, for easier sending of multiple emails.

Feature Requests and Ideas :
CSS Parsing.
PDF - Watermarking.
IMAP - read folders, read messages, move messages, delete messages.
Filter on the File lists to only allow selecting various file formats.
Documenting the error codes to give a bit more info about which library is throwing the error.
BarCode Scanning - http://zbar.sourceforge.net/ or https://github.com/zxing
Printer selection.
Extend the install script step stuff, to do binary data, Add a Clear all function, Add a check to see if a function is installed or not.
Duktape module support for loading external modules in the JS function.

Previous Ideas no longer in development :
JSON wildcard support - our base library didn't allow this, and so it wasn't included and now JSON support is native in FMP.  The better approach would be to request this within FileMaker.
HTML to PDF - the new print functions on Server and WebDirect negate the need for this to a large extent.
